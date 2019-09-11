void betbos::allocate_bets( const vector<name> candidates ) {
    contr_config conf = configs();
    check(candidates.size() == conf.numelected, "ERR::NEWTENURE_CANDIDATES_NUM_ELECTED::Number of candidates does not match the threshold defined in `numelected` configs");

    // Empty the bets table to get a full set of new bets.
    auto bet_itr = _bets.begin();
    while (bet_itr != _bets.end()) {
        auto candidates_itr = _candidates.find(bet_itr->bet_name.value);
        check( candidates_itr != _candidates.end() , "ERR::NEWTENURE_EXPECTED_CAND_NOT_FOUND::Corrupt data: Trying to set a lockup delay on candidate leaving office.");

        _candidates.modify(candidates_itr, eosio::same_payer, [&](auto & row) {
            eosio::print("Lockup stake for release delay.");
            row.unstaking_end_time_stamp = current_time_point() + time_point_sec(conf.lockup_release_time_delay);
        });
        bet_itr = _bets.erase(bet_itr);
    }

    // Add appointed candidates as bets
    for (const name candidate_name : candidates) {
        // update bet's locked token time period
        auto candidates_itr = _candidates.find(candidate_name.value);
        check( candidates_itr != _candidates.end() , "ERR::NEWTENURE_EXPECTED_CAND_NOT_FOUND::Corrupt data: Trying to set a lockup delay on candidate leaving office.");

        // Check if candidate has the proper locked assets
        const asset locked_tokens = candidates_itr->locked_tokens;
        const asset mininum_locked_tokens = conf.lockupasset;
        check(locked_tokens.amount >= mininum_locked_tokens.amount, "ERR::NEWTENURE_CANDIDATE_REQUIRE_LOCKED_TOKENS::Candidate does not meet the minimum `locked_tokens` threshold.");
        check(candidates_itr->is_active, "ERR::NEWTENURE_CANDIDATE_ACTIVE::Candidate must be active to be elected as an bet.");

        // Add Candidate name to bet table
        _bets.emplace( get_self(), [&](auto & row) {
            row.bet_name = candidate_name;
        });
        // Lockup stake for release delay.
        _candidates.modify(candidates_itr, eosio::same_payer, [&](auto & row) {
            row.unstaking_end_time_stamp = current_time_point() + time_point_sec(conf.lockup_release_time_delay);
        });
    }
}

void betbos::set_bet_auths() {
    name account_to_change = configs().authaccount;

    vector<eosiosystem::permission_level_weight> accounts;

    for (auto bets_itr = _bets.begin(); bets_itr != _bets.end(); bets_itr++) {
        eosiosystem::permission_level_weight account{
                .permission = eosio::permission_level(bets_itr->bet_name, "active"_n),
                .weight = (uint16_t) 1,
        };
        accounts.push_back(account);
    }

    eosiosystem::authority bets_contract_authority{
            .threshold = configs().auth_threshold_bets,
            .keys = {},
            .accounts = accounts
    };

    action(permission_level{account_to_change, "active"_n},
           "eosio"_n, "updateauth"_n,
           std::make_tuple(
                   account_to_change,
                   AUDITORS_PERMISSION,
                   "active"_n,
                   bets_contract_authority))
            .send();
}

void betbos::newtenure( const vector<name> candidates, const string message) {
    require_auth( get_self() );

    // Set bets for the next period.
    allocate_bets(candidates);

    // Set the auths on the BOS bet authority account
    set_bet_auths();
}
