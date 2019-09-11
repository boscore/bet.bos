<h1 class="contract">updatebio</h1>

## Description

To allow a candidate update their bio information after they have registerd. The action ensures the user has agreed to the latest terms and conditions, has the correct authorization of the {{ cand }} to perform the action and is already registerd as a candidate. Then the bio information for the candidate will be updated leaving all other data of the candidate unchanged.

<h1 class="contract">firebet</h1>

## Description

To allow elected bets to (where quorum and configured majorities are met) to remove a fellow bet and lock up their tokens until the configured delay period has passed.

<h1 class="contract">resign</h1>

## Description

To remove an elected bet. This action must be run by the resigning bet and the outcome should remove the elected bet and lock up their tokens until the delay period has passed so the tokens can be claimed with the unstake action.

<h1 class="contract">firecand</h1>

## Description

The intent of forehand is to set a candidate to a state of inactive so they will be excluded from the next election round. This action may only be run by the by elected bets (where quorum and configured majorities are met). There is an option to lock up the candidate's tokens until a delay period has passed based on the delay set in the config after which the tokens can be claimed with the unstake action. If the option passed is false and there is an existing lockup delay on the tokens then this lockup will continue to be active until the lock up time has passed.

<h1 class="contract">unstake</h1>

## Description

To return staked tokens back to the candidate if the user is no longer an active candidate and there is no delay set on the candidate the staked tokens will be returned to the candidate.
**TERM:** This action lasts for the duration of the time taken to process the transaction.

<h1 class="contract">updateconfig</h1>

Update the configuration for the running contract of selected parameters without needing change the source code. This requires a privileged account.

<h1 class="contract">registercand</h1>

### V1.0 Auditor’s Declaration of Independence and Impartiality

I, {{ registercand }}, accept to serve as Auditor, in accordance with the BOS Rules.

I
**DECLARE** to be and to intend to remain independent and impartial during the auditing procedure.

**DECLARE** that, to my knowledge, there are no facts, circumstances or relationships which may affect my independence and impartiality.

**DECLARE** to treat all BOS members fairly, reward contributions appropriately and not seek unmerited profits. No member should have less or more information about an auditing decision than others.

**DECLARE** not to seek any stake in, or exert undue influence over, other block producers and shall take appropriate measures to protect my own independence and impartiality.

## Description

The intent of {{ registercand }} is to registers a candidate to bet election, Accounts must register as a candidate before they can be voted for. The candidate must lock a configurable number of tokens before trying to register (configurable via {{ updateconfig }} in the parameter lockupasset which will be sent from the token contract as defined and set in the code of the contract. If a user previously been a candidate they may have enough staked tokens to not require further staking but will otherwise need to transfer the difference to meet the required stake.

<h1 class="contract">withdrawcand</h1>

## Description

To withdraw a candidate for becoming an elected bet. The action ensures the {{ cand }} account is currently registerd. On success the amount of tokens that was locked up via the {{ registercand }} action will be added to a list of pending transactions to transfer back to the {{ cand }} account. The actual transfer would be performed by a separate action due to the auth requirement for sending funds from the contract's account.

<h1 class="contract">vote</h1>

## Description

To allow a member of BOS to vote for candidates that are eligible become bets after the next call to {{ newtenure }}. The action ensures the user has agreed to the latest terms and conditions and has the correct authorization of the account: {{ voter }} to place or change an active vote. Upon success this action will either update an existing vote with a new set of candidates or create a new active vote for the {{ voter }} for candidates eligible for election.

<h1 class="contract">newtenure</h1>

## Description

To signal the end of one election period and commence the next. It performs several actions after the following conditions are met:

- The action is not called before the period should have ended
- Enough voter value has participated to trigger the initial running of the BOS
- After BOS bets has started enough voter value has continued engagement with the BOS bet voting process.

1. Set the permissions for the elected bets so they have sufficient permission to run the BOS bet permission according to the constitution and technical permissions design.
2. Set the time for the beginning of the next period to mark the reset anniversary for the BOS bet elections.

<h1 class="contract">claimpay</h1>

## Description

The intent of {{ claimpay }} is to allow an account to claim pending payment amounts due to the account. The pay claim they are claiming needs to be visible in the `pendingpay` table. Transfers to the claimer via an inline transfer on the `eosio.token` contract and then removes the pending payment record from the `pending_pay` table. The active auth of this claimer is required to complete this action.

<h1 class="contract">unvote</h1>

## Description

Removes existing vote from {{ voter }}.

<h1 class="contract">cleancand</h1>

## Description

> Used to refresh `candidate` data entry
> Authorized by `require_auth( _self )`

- set `total_votes` to 0
- set `is_active` if locked_tockens met minimum threshold

<h1 class="contract">cleanvoter</h1>

## Description

> Used to refresh `voter` data entry
> Authorized by `require_auth( _self )`

- If voter has not voted for any candidates, remove voter from `votes` & `votejson`
- Update voter's staked & proxy data
- Add `vote_json` if not present in `votejson` table
