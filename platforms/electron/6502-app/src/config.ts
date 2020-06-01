//
//  config.ts
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 27 May 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//


// file format ---------------------------------------------------------------------------------------------------------

const file = {
    defaultExtension: 'a65',
    filters:          [
        {name: '6502 Assembly Language File',  extensions: ['a65', 'a6502', '65s', '6502s']},
        {name: 'Other Assembly Language File', extensions: ['asm', 'a65C', 'a65C02', 's']},
        {name: 'All Files',                    extensions: ['*']},
    ],
};


// export --------------------------------------------------------------------------------------------------------------

export default {
    file,
};
