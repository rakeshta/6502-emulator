//
//  assembler.ts
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 10 Jun 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//

import AssemblerWorker  from 'worker-loader!./assembler.worker';


// types ---------------------------------------------------------------------------------------------------------------

export interface SourceCode {
    versionId:   string;
    value:       string;
}


//
