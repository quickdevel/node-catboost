# CatBoost Wrapper for Node.js

Module uses [CatBoost C/C++ library](https://tech.yandex.com/catboost/doc/dg/concepts/c-plus-plus-api_dynamic-c-pluplus-wrapper-docpage/) (catboostmodel.dll/libcatboostmodel.so).

## Installation

For install CatBoost wrapper for Node.js use NPM:

    npm install catboost

## Example

Single object:

    const catboost = require('catboost')
    
    const model = new catboost.Model('flats.bin')
    
    const price = model.calc([
      'Moscow', // city
      2, // rooms
      50, // square
      9, // floor
      12 // total floors
    ])
    console.log(`Flat price: ${price.toFixed()} RUR`)

Many objects:

    const catboost = require('catboost')
    
    const model = new catboost.Model('flats.bin')
    
    const prices = model.calcMany([
      [
        'Moscow', // city
        2, // rooms
        50, // square
        9, // floor
        12 // total floors
      ],
      [
        'Yekaterinburg', // city
        1, // rooms
        35, // square
        2, // floor
        9  // total floors
      ]
    ])
    console.log(`Flat prices: ${price[0].toFixed()} RUR and ${price[1].toFixed()} RUR`)
