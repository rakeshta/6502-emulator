//
//  Assembly.ts
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 15 Jun 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//


export interface Message {
    lineNumber:  number;
    type:       'error';
    text:        string;
}

export default interface Assembly {
    versionId:   number;
    success:     boolean;
    messages:    Message[];
}
