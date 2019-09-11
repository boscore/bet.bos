void betbos::registercand( const name cand ) {
    require_auth( cand );

    const auto candidate_itr = _candidates.find(cand.value);

    check( candidate_itr != _candidates.end(), "ERR::NOMINATECAND_NOT_FOUND::Cannot find candidate to register, candidate must first transfer BOS tokens to `bet.bos` account." );
    check( !candidate_itr->is_active, "ERR::NOMINATECAND_ALREADY_REGISTERED::Candidate is already registered and active." );
    check( candidate_itr->locked_tokens >= configs().lockupasset, "ERR::NOMINATECAND_INSUFFICIENT_FUNDS_TO_STAKE::Insufficient funds have been staked." );

    // Set candidate as active
    _candidates.modify( candidate_itr, get_self(), [&](auto & row) {
        row.is_active = 1;
    });
}

void betbos::withdrawcand( const name cand ) {
    require_auth( cand );
    invalid_candidate( cand, false );
}

void betbos::firecand( const name cand ) {
    require_auth( configs().authaccount );
    invalid_candidate( cand, false );
}

void betbos::unstake( const name cand ) {
    require_auth( cand );

    const auto candidate_itr = _candidates.find(cand.value);

    check( candidate_itr != _candidates.end(), "ERR::UNSTAKE_CAND_NOT_REGISTERED::Candidate is not already registered." );
    check( !candidate_itr->is_active, "ERR::UNSTAKE_CANNOT_UNSTAKE_FROM_ACTIVE_CAND::Cannot unstake tokens for an active candidate. Call withdrawcand first." );
    check( candidate_itr->unstaking_end_time_stamp < time_point_sec(current_time_point()), "ERR::UNSTAKE_CANNOT_UNSTAKE_UNDER_TIME_LOCK::Cannot unstake tokens before they are unlocked from the time delay." );
    check( candidate_itr->locked_tokens.amount > 0, "ERR::UNSTAKE_ZERO_LOCKED_TOKENS::Cannot unstake tokens of zero balance." );

    _candidates.modify(candidate_itr, get_self(), [&](auto & row) {
        // Ensure the candidate's tokens are not locked up for a time delay period.
        // Send back the locked up tokens
        // inline transfer unstaking
        eosio::action(
                eosio::permission_level{get_self() , "active"_n },
                name( TOKEN_CONTRACT ),
                "transfer"_n,
                make_tuple( get_self(), cand, row.locked_tokens, string("Returning locked up stake. Thank you."))
        ).send();

        row.locked_tokens = asset(0, configs().lockupasset.symbol);
    });
}

void betbos::resign( const name bet ) {
    require_auth( bet );
    remove_bet( bet );
}

void betbos::firebet( const name bet ) {
    require_auth( configs().authaccount );
    remove_bet( bet );
}

// private methods for the above actions

void betbos::remove_bet( const name bet ) {
    const auto bet_itr = _bets.find(bet.value);
    check(bet_itr != _bets.end(), "ERR::REMOVEBET_NOT_CURRENT_BET::The entered account name is not for a current bet.");

    // Remove bet from the bets table.
    _bets.erase(bet_itr);

    // Remove the candidate from being eligible for the next election period.
    invalid_candidate(bet, true);

    // Update the auths to give control to the new set of bets.
    set_bet_auths();
}

void betbos::invalid_candidate( const name cand, const bool lockupStake ) {
    const auto candidate_itr = _candidates.find(cand.value);
    check(candidate_itr != _candidates.end(), "ERR::INVALID_CANDIDATE_NOT_FOUND::Candidate to invalidate is not registered.");

    eosio::print("Remove from registerd candidate by setting them to inactive.");

    // Set the is_active flag to false instead of deleting in order to retain votes if they return as BOS bets.
    _candidates.modify(candidate_itr, cand, [&](auto & row) {
        row.is_active = 0;
        if (lockupStake) {
            eosio::print("Lockup stake for release delay.");
            row.unstaking_end_time_stamp = current_time_point() + time_point_sec(configs().lockup_release_time_delay);
        }
    });
}
