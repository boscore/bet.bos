void betbos::transfer( name from,
                           name to,
                           asset quantity,
                           const std::string& memo ) {

    if (to == get_self() && from != "ops.bos"_n && from != "bos"_n && from != "kyc.bos"_n && from != "bet.bos"_n && from != "escrow.bos"_n && from != "auditor.bos"_n && from != "dappfund.bos"_n) {
        const auto & candidate_itr = _candidates.find(from.value);
        const time_point_sec unstaking_period = current_time_point() + time_point_sec(configs().lockup_release_time_delay);

        bool is_active = false;

        // Modify existing registered candidate with additional locked_tokens
        if (candidate_itr != _candidates.end()) {
//            if (candidate_itr->locked_tokens + quantity >= configs().lockupasset) is_active = true;  // Auto register candidate if locked_tockens is above minimum threshold
            // Modify locked_tokens amount & set new unstaking period
            _candidates.modify(candidate_itr, eosio::same_payer, [&](auto & row) {
                row.locked_tokens += quantity;
                row.unstaking_end_time_stamp = unstaking_period;
                row.is_active = is_active;
                row.total_votes = 0;
            });
        // New candidate, register them and add locked_tokens
        } else {
//            if (quantity >= configs().lockupasset) is_active = true;            // Auto register candidate if locked_tockens is above minimum threshold

            // Add candidate to table
            _candidates.emplace( get_self(), [&](auto & row) {
                row.candidate_name = from;
                row.locked_tokens = quantity;
                row.total_votes = 0;
                row.is_active = is_active;
                row.unstaking_end_time_stamp = unstaking_period;
            });
        }
    }
}
