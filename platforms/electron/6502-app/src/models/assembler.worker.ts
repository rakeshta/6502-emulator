//
//  assembler.worker.ts
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 10 Jun 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//

import dasm   from 'dasm';


// eslint-disable-next-line @typescript-eslint/no-explicit-any
const ctx: Worker = self as any;


ctx.addEventListener('message', (event) => {
    console.log('--debug worker: [message]', event);
    ctx.postMessage({foo: 'hello world'});
});
