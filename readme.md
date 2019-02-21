# How to use

Send message
```javascript 1.7
  const eos = scatter.eos(network, EosJS, {});
  eos.transaction({
    actions: [{
      account: chat_contract,
      name: 'addmessage',
      authorization: [{
        actor: userName,
        permission: 'active',
      }],
      data: {
        sender: userName,
        recipient: userName,
        sms: message
      },
    }]
  }, {
    blocksBehind: 3,
    expireSeconds: 30,
  });
```


Read messages
```javascript 1.7
  eosjs.getTableRows({
      json: true,
      code: chat_contract,
      scope: chat_contract,
      table: 'messages',
    }).then(res => {
      console.log(this.rows);
    }).catch(e => {
      console.error(e);
    });
  };
```