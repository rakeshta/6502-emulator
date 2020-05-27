//
//  config.ts
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 27 May 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//


// file format ---------------------------------------------------------------------------------------------------------

const file = {
    defaultExtension: '65s',
    filters:          [
        {name: '6502 Assembly Language File',  extensions: ['65s', '6502s']},
        {name: 'Other Assembly Language File', extensions: ['asm', 's']},
        {name: 'All Files',                    extensions: ['*']},
    ],
};


// export --------------------------------------------------------------------------------------------------------------

export default {
    file,
};
