# `bet.bos` - BOS Bet Elections Contract

This contract will be in charge of bet registration and voting for BOS bets.

When an bet registers, they need to provide their bio by calling the `updatebio` action.

If an elected bet resigns via the `withdrawcand` during a period a new candidate will be chosen to fill the gap on the bet board from the votes ranking in the candidates at that moment.

## Quickstart

### ACTION `eosio.token::transfer` & `registercand`

> Nominate Candidate

> Must `eosio::transfer` BOS tokens at a minimum of `lockupasset` (10000.0000 BOS).
> Once candidate has meet the minimum locked tokens threshold, they will be automatically set as an active candidate able to receive votes from BOS users.

```bash
$ bosc transfer <CANDIDATE> bet.bos "10000.0000 BOS" -m "stake for bet.bos"
```
OR
```bash
$ bosc tx create bet.bos registercand '{"cand": "<CANDIDATE NAME>"}' -p <CANDIDATE NAME>@active
```

### ACTION `vote`

> Vote for Bet Candidate

```
$ bosc tx create bet.bos vote '{"voter":"<VOTER>","newvotes":["<CANDIDATE_1>", "<CANDIDATE_2>","<CANDIDATE_3>"], "vote_json":""}' -p deniscarrier
```

### ACTION `unvote`

> Removes existing vote from `voter`.

```bash
$ bosc tx create bet.bos unvote '{"voter": "<VOTER NAME>"}' -p <VOTER NAME>@active
```

### Withdraw Candidate & Unstake

> A Candidate must `withdrawcand` to be allowed to `unstake` any BOS assets that was sent to `bet.bos`.

```
$ bosc tx create bet.bos withdrawcand '{"cand":"<CANDIDATE>"}' -p <CANDIDATE>
$ bosc tx create bet.bos unstake '{"cand":"<CANDIDATE>"}' -p <CANDIDATE>
```

### ACTION `resign`

> Resign as Bet

> Removes elected bet from `bet.bos@bets`

```
$ bosc tx create bet.bos resign '{"bet":"<BET>"}' -p <BET>
```

### ACTION `newtenure`

> Start Bet Tenure (Election)

> `bet.bos@bets` permission will `updateauth` with candidates with the highest votes

```bash
$ bosc tx create bet.bos newtenure '{"candidates": ["<CANDIDATE 1>", "<CANDIDATE 2>"], "message":"newtenure for bet.bos"}' -p bet.bos@active
```

### ACTION `firebet`

> Removes Bet from `bet.bos@bets` authority

```bash
$ bosc tx create bet.bos firebet '{"bet": "<BET NAME>"}' -p bet.bos@active
```

### ACTION `resign`

> Removes Bet from `bet.bos@bets` authority

```bash
$ bosc tx create bet.bos resign '{"bet": "<BET NAME>"}' -p <BET NAME>@active
```

### ACTION `cleancand`

> Used to clean `candidate` data entry
> Authorized by `require_auth( _self )`
- set `total_votes` to 0
- set `is_active` if locked_tockens met minimum threshold

```bash
$ bosc tx create bet.bos cleancand '{"cand": "<CANDIDATE NAME>"}' -p bet.bos@active
```

### ACTION `cleanvoter`

> Used to clean `voter` data entry
> Authorized by `require_auth( _self )`
- If voter has not voted for any candidates, remove voter from `votes` & `votejson`
- Update voter's staked & proxy data
- Add `vote_json` if not present in `votejson` table

```bash
$ bosc tx create bet.bos cleanvoter '{"voter": "<VOTER NAME>"}' -p bet.bos@active
```

## Tables

### candidates

 - `candidate_name` (name) - Account name of the candidate
 - `locked_tokens` (asset) - An asset object representing the number of tokens locked when registering
 - `total_votes` (uint64) - Updated tally of the number of votes cast to a candidate. This is updated and used as part of the `newtenure` calculations. It is updated every time there is a vote change or a change of token balance for a voter for this candidate to facilitate live voting stats.
 - `is_active` (bool) - Boolean indicating if the candidate is currently available for election.
 - `unstaking_end_time_stamp` (time_point_sec) - timestamp that user is allowed to unstake tokens.

### bets

- `bet_name` (name) - Account name of the bet

### votes

 - `voter` (name) - The account name of the voter
 - `proxy` (name) - DEPRECATED: the proxy set by the voter, if any (not being used to count final vote tally)
 - `staked` (uint64) - DEPRECATED: total staked amount of voter (not being used to count final vote tally)
 - `candidates` (name[]) - The candidates voted for, can supply up to the maximum number of votes (currently 5) - Can be configured via `updateconfig`

### votejson

 - `voter` (name) - The account name of the voter
 - `vote_json` (string JSON)- JSON metadata for voting (ex: {"comment": "great work"})
 - `updated_at` (time_point_sec) last updated timestamp

## bios

 - `candidate_name` (name) - Account name of candidate
 - `bio` (string JSON) - Bio of candidate

### config

- `lockupasset` (asset) -  The amount of assets that are locked up by each candidate applying for election.
- `maxvotes` (int default=3) - The maximum number of votes that each member can make for a candidate.
- `numelected` (int default=5) -  Number of bets to be elected for each election count.
- `authaccount` ( account= "bet.bos") - account to have active auth set with all bets on the newtenure.
- `auth_threshold_bets` (uint8) - Number of bets required to approve the lowest level actions.
- `lockup_release_time_delay` (date) - The time before locked up stake can be released back to the candidate using the unstake action

## Actions

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

### V1.0 BET's Declaration of Independence and Impartiality

I, {{ registercand }}, accept to serve as Bet, in accordance with the BOS Rules.

I have consented to act as a BOS Executive Team member, and I agree to comply with the following BET Members’ Code of Ethics throughout my term as a member:

**Honesty and Good Faith** – I will act honestly and in good faith. I will do nothing to violate the trust of the community I serve.


**Care, Diligence and Skill** – I will exercise the degree of care, diligence and skill of a reasonably prudent person in comparable circumstances. I will make a concerted effort to attend all scheduled BET meetings. I will act responsibly and with due diligence to become familiar with the affairs of the BOS ecosystem.


**Conflict of Interest** – I am not currently aware of any actual or potential conflict of interest with respect to the BOS BET team or any matter detrimental to BOS. If I become aware of any conflict, I will immediately disclose it to the Chairpersons. I will not promote my own interests or those of any friend to the detriment of the BOS ecosystem. I will not seek any special benefits or privileges as a BET member. I will act only in the best interests of the BOS ecosystem as a whole and I will not favour the interests of any individual or group of token holders.


**Confidentiality** – I will not disclose to any person information decided by the BOS BET members to be confidential or privileged or which reasonably ought to be deemed confidential. When in doubt, I will request determination by a resolution of the Board.


**Good Conduct** – At all times, I will conduct myself in a professional and businesslike manner at meetings of the Board. I will approach all Board issues with an open mind, preparing to make the best decisions on behalf of the BOS ecosystem. I will act ethically with integrity. I will deal with others in a respectful manner.
 
**Support** – I will abide by decisions of the Board even though I may disagree, but I reserve the right to express my own views to my community upon non-confidential issues.


**Defamation** – I will not make erroneous or defamatory statements about the Board.


**Agreement –** I hereby agree to comply with the provisions set out in this BET Members’ Code of Ethics.

 

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

## Description

MAINTENANCE action used to refresh {{ cand }}

<h1 class="contract">refreshvoter</h1>

## Description

MAINTENANCE action used to refresh {{ voter }}
