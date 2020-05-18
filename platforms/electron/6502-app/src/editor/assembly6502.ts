//
//  assembly6502.tsx
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 16 May 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//

import * as monaco  from 'monaco-editor';

import IMonarchLanguage = monaco.languages.IMonarchLanguage;


// types ---------------------------------------------------------------------------------------------------------------

interface Assembly6502MonarchLanguage extends IMonarchLanguage {
    id:              string;
    symbols:         RegExp;
    operators:       string[];
    keywords:        string[];
    instructions:    string[];
    exInstructions:  string[];
}


// definition ----------------------------------------------------------------------------------------------------------

const lang: Assembly6502MonarchLanguage = {

    id:           'assembly6502',

    ignoreCase:    true,
    defaultToken: 'invalid',

    brackets: [
        { open: '{', close: '}', token: 'delimiter.curly' },
		{ open: '[', close: ']', token: 'delimiter.bracket' },
		{ open: '(', close: ')', token: 'delimiter.parenthesis' }
    ],


	symbols: /[=><!~?:&|+\-*/^%.#]+/,
    operators: [
        '=', '+', '-', '<<', '>>', '~', '&', '|', '#',
    ],

    keywords: [
        'processor',
        'org',
        '.byte',
        '.word',
        ',x',
        ',y',
    ],

    instructions: [
        'ADC', 'AND', 'ASL', 'BCC', 'BCS', 'BEQ', 'BIT', 'BMI', 'BNE', 'BPL', 'BRK', 'BVC', 'BVS', 'CLC', 'CLD', 'CLI',
        'CLV', 'CMP', 'CPX', 'CPY', 'DEC', 'DEX', 'DEY', 'EOR', 'INC', 'INX', 'INY', 'JMP', 'JSR', 'LDA', 'LDX', 'LDY',
        'LSR', 'NOP', 'ORA', 'PHA', 'PHP', 'PLA', 'PLP', 'ROL', 'ROR', 'RTI', 'RTS', 'SBC', 'SEC', 'SED', 'SEI', 'STA',
        'STX', 'STY', 'TAX', 'TAY', 'TSX', 'TXA', 'TXS', 'TYA',
    ],

    exInstructions: [
        'KIL', 'SLO', 'RLA', 'SRE', 'RRA', 'SAX', 'LAX', 'DCP', 'ISC', 'ANC', 'ALR', 'ARR', 'XAA', 'AXS', 'AHX', 'SHY',
        'SHX', 'TAS', 'LAS',
    ],

    tokenizer: {

        root: [
            {include: '@whitespace'},
            {include: '@number'},
            {include: '@string'},
            {include: '@bracket'},
            {include: '@symbol'},
            {include: '@keyword'},
        ],

        whitespace: [
            [/[ \t\r\n]+/, 'white'],
            [/;.*$/,    'comment'],
        ],

        number: [
            [/\$([abcdef]|[ABCDEF]|\d)+/, 'number.hex'],
            [/%[01]+/, 'number.binary'],
            [/\d+/, 'number'],
        ],

        string: [
            [/'.*'/, 'string'],
        ],

        bracket: [
            [/[{}()[\]]/, '@brackets'],
        ],

        symbol: [
            [/@symbols/, {
                cases: {
                    '@operators': 'operator',
                    '@default': ''
                }
            }],
        ],

        keyword: [
            [/[.,]?[a-zA-Z_]\w*/, {cases: {
                '@keywords':       'keyword',
                '@instructions':   'instruction',
                '@exInstructions': 'exInstruction',
                '@default':        'identifier',
            }}]
        ],
    },
};


// register language ---------------------------------------------------------------------------------------------------

monaco.languages.register({id: lang.id});
monaco.languages.setMonarchTokensProvider(lang.id, lang);
