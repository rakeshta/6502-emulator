//
//  assembler.ts
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 10 Jun 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//

import AssemblerWorker  from 'worker-loader!./assembler.worker';

import WorkerMessage    from '../types/WorkerMessage';
import WorkerResponse   from '../types/WorkerResponse';
import SourceCode       from '../types/SourceCode';
import Assembly         from '../types/Assembly';


// worker --------------------------------------------------------------------------------------------------------------

const worker = new AssemblerWorker();

let callbackCounter = 0;
const callbacks: Record<number, (res?: unknown) => void> = {};

function postMessage<Data, Response>(message: string, data: Data): Promise<Response> {
    const id = callbackCounter++;
    return new Promise((resolve) => {
        callbacks[id] = resolve as (res?: unknown) => void;
        worker.postMessage({id, message, data} as WorkerMessage<Data>);
    });
}

worker.addEventListener('message', (event) => {

    // get callback for this message
    const data = event.data as WorkerResponse<unknown>;
    const cb   = callbacks[data.id];

    // balk if callback not found
    if (!cb) {
        throw new Error(`Invalid callback message id [${data.id}] from worker`);
    }

    // invoke callback & cleanup
    cb(data.result);

    delete callbacks[data.id];
});


// interface -----------------------------------------------------------------------------------------------------------

export default {

    async assemble(code: SourceCode): Promise<Assembly> {
        return postMessage('assemble', code);
    },
};
