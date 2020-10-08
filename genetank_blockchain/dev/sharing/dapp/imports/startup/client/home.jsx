//file home.jsx Author:sz Version:0.1
import React from 'react'
import { AccountForm, BlockNumForm, SmartContract } from './blockchnop.jsx'
import { Transactions } from './wallet.jsx'

export const Home = () => (
  <div>
     <h2 className="page-header">My GTT Wallet</h2>
     <fieldset>
       <legend>Blockchain account information</legend>
       <AccountForm />
       <Transactions />
     </fieldset>
     <p />
     <fieldset>
       <legend>Switch smart contracts</legend>
       <SmartContract />
     </fieldset>
     <p />
     <fieldset>
       <legend>Blockchain browser</legend>
       <BlockNumForm />
     </fieldset>
  </div>
)
