//
//  assembler.worker.ts
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 10 Jun 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//

import dasm            from 'dasm';

import WorkerMessage   from '../types/WorkerMessage';
import WorkerResponse  from '../types/WorkerResponse';
import SourceCode      from '../types/SourceCode';
import Assembly        from '../types/Assembly';


// eslint-disable-next-line @typescript-eslint/no-explicit-any
const ctx: Worker = self as any;


const handlers: Record<string, Function> = {
    assemble(code: SourceCode): Assembly {
        const res = dasm(code.value, {format: 2});
        console.log('--debug assembly result', res);
        return {versionId: code.versionId, success: res.success};
    },
};


ctx.addEventListener('message', (event) => {

    const {id, message, data} = event.data as WorkerMessage<unknown>;
    const func = handlers[message];

    if (!func) {
        throw new Error(`Invalid message to worker [${message}]`);
    }

    const result = func(data);
    const response: WorkerResponse<unknown> = {id, result};
    ctx.postMessage(response);
});
