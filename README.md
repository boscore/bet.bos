# BET  Contract design Guideline

# Part I.  Account and Permissions

bet: BOS Executive Team 
### `bet.bos` 
**Permission**

```
owner (eosio@active)
active
  (13/25)
  eosio.code@active
minority (9/25)
```

**Function**

- transfer fund to escrow.bos for each proposal
- transfer stipend to betchair.bos
- transfer fund for bet operation, developers, auditors to betchair.bos

### betchair.bos`

**Permission**

```
owner (bet.bos@active)
active (yves@active)
```
**Function**
distribute stipend to each bet member every 3 months
distribute fund for bet operations  (0.8% inflation)
distribute fund for developers/infrastructure (0.15% inflation)
distribute fund for auditors (0.05% inflation)


# Part II：ACTION Design
## regbetcand [declaration]
Function:     
* This action is used to register a candidate for bet elections.
* The candidate must have transferred a quantity of tokens (determined by a config setting - `lockupasset`) to the contract for staking before this action is executed. This could have been from a recent transfer with the contract name in the memo or from a previous time when this account had nominated, as long as the candidate had never `unstake`d those tokens.

Permission: anyone
Parameter:
```
name cand
```



## withdrawcand

Function:     

* This action is used to withdraw a candidate from being active for auditor elections.

Permission: the candidate itself

Parameters:

```
name cand
```



## votebetcand

Function:
* This action is to facilitate voting for candidates to become bet board of BOS.


Permission: anyone

Parameters:

```
name voter, std::vector<name> newvotes
```



## rmbet [include resign]

Function: To remove a BET members
Permission: `bet.bos@active` (majority decision)
Parameters:

```
name  betmember  //majority decision
```

## rmabsentbet

Function: To remove a BET members who miss two (2) or more consecutive scheduled meetings by minority decision.
Permission: `bet.bos@minority` (minority decision)
Parameters:

```
name  betmember  //majority decision
```



## regbetchair [declaration]

Function:     

- This action is used to register a chairman for bet chairman elections.
- The candidate must have transferred a quantity of tokens (determined by a config setting - `lockupasset`) to the contract for staking before this action is executed. This could have been from a recent transfer with the contract name in the memo or from a previous time when this account had nominated, as long as the candidate had never `unstake`d those tokens.

Permission: anyone
Parameter:

```
name cand
```



## votechairman 

Function: 
- This action is to facilitate voting for candidates to become bet mem of BOS.
- After vote internal, set chairman for BET Board

Permission: `bet.bos@active` (majority decision)
Parameters:

```
name  chair_cand  //chairman candidate
```

## rmchairman [include resign]
Function: to remove the current chairman and replace it with `bet.bos@active`

Permission: `bet.bos@active` (majority decision)
Parameters:

```
name  chairman  //majority decision
```



  

## transfer [listen] 
Function:
* Action to listen to from the associated token contract to ensure registering should be allowed.
* This action is intended only to observe transfers that are run by the associated token contract for the purpose of tracking the moving weights of votes if either the `from` or `to` in the transfer have active votes. It is not included in the ABI to prevent it from being called from outside the chain.





# Part III：Table Design



## TABLE betcands
Function：BET Candidate 
parameters：

```
name candidate_name;
    asset locked_tokens;
    uint64_t total_votes;
    uint8_t is_active;
    time_point_sec auditor_end_time_stamp;
```



## TABLE betchaircands

Function：BET Chair Candidate
parameters：

```
name candidate_name;
```



# Part IV: Declaration

## regbetcand

I {{regbetcand}}, have consented to act as a BOS Executive Team member, and I agree to comply with the following BET Members’ Code of Ethics throughout my term as a member:

***Confidentiality***  – I DECLARE not to disclose to any person information decided by the BOS BET members to be confidential or privileged or which reasonably ought to be deemed confidential. When in doubt, DECLARE to request determination by a resolution of the Board.

***Good Conduct***  – At all times, I DECLARE to conduct myself in a professional and businesslike manner at meetings of the Board. DECLARE to approach all Board issues with an open mind, preparing to make the best decisions on behalf of the BOS ecosystem. DECLARE to act ethically with integrity. DECLARE to deal with others in a respectful manner.

***Support***  – I DECLARE to abide by decisions of the Board even though I may disagree, but I reserve the right to express my own views to my community upon non-confidential issues.

*** Defamation***  – I DECLARE not to make erroneous or defamatory statements about the Board.

***Agreement ***  I hereby agree to comply with the provisions set out in this BET Members’ Code of Ethics.





## regbetchair

