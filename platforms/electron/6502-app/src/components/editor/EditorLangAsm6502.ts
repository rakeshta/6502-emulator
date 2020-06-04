//
//  EditorLangAsm6502.ts
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 29 May 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//

import * as monaco    from 'monaco-editor';


import IMonarchLanguage             = monaco.languages.IMonarchLanguage;
import CompletionItemProvider       = monaco.languages.CompletionItemProvider;
import CompletionList               = monaco.languages.CompletionList;
import CompletionItem               = monaco.languages.CompletionItem;
import CompletionItemInsertTextRule = monaco.languages.CompletionItemInsertTextRule;


// types ---------------------------------------------------------------------------------------------------------------

interface Instruction {
    [mnemonic: string]: string;
}

interface Snippet {
    label:    string;
    text:     string;
}

interface Asm6502MonarchLanguage extends IMonarchLanguage {
    id:              string;
    symbols:         RegExp;
    operators:       string[];
    keywords:        string[];
    instructions:    string[];
    exInstructions:  string[];
}


// lang ----------------------------------------------------------------------------------------------------------------

const instructions: Instruction[] = [
    {ADC: 'Add with carry'},
    {AND: 'And (with accumulator)'},
    {ASL: 'Arithmetic shift left'},
    {BCC: 'Branch on carry clear'},
    {BCS: 'Branch on carry set'},
    {BEQ: 'Branch on equal (zero set)'},
    {BIT: 'Bit test'},
    {BMI: 'Branch on minus (negative set)'},
    {BNE: 'Branch on not equal (zero clear)'},
    {BPL: 'Branch on plus (negative clear)'},
    {BRK: 'Break / interrupt'},
    {BVC: 'Branch on overflow clear'},
    {BVS: 'Branch on overflow set'},
    {CLC: 'Clear carry'},
    {CLD: 'Clear decimal'},
    {CLI: 'Clear interrupt disable'},
    {CLV: 'Clear overflow'},
    {CMP: 'Compare (with accumulator)'},
    {CPX: 'Compare with X'},
    {CPY: 'Compare with Y'},
    {DEC: 'Decrement'},
    {DEX: 'Decrement X'},
    {DEY: 'Decrement Y'},
    {EOR: 'Exclusive or (with accumulator)'},
    {INC: 'Increment'},
    {INX: 'Increment X'},
    {INY: 'Increment Y'},
    {JMP: 'Jump'},
    {JSR: 'Jump subroutine'},
    {LDA: 'Load accumulator'},
    {LDX: 'Load X'},
    {LDY: 'Load Y'},
    {LSR: 'Logical shift right'},
    {NOP: 'No operation'},
    {ORA: 'Or with accumulator'},
    {PHA: 'Push accumulator'},
    {PHP: 'Push processor status (SR)'},
    {PLA: 'Pull accumulator'},
    {PLP: 'Pull processor status (SR)'},
    {ROL: 'Rotate left'},
    {ROR: 'Rotate right'},
    {RTI: 'Return from interrupt'},
    {RTS: 'Return from subroutine'},
    {SBC: 'Subtract with carry'},
    {SEC: 'Set carry'},
    {SED: 'Set decimal'},
    {SEI: 'Set interrupt disable'},
    {STA: 'Store accumulator'},
    {STX: 'Store X'},
    {STY: 'Store Y'},
    {TAX: 'Transfer accumulator to X'},
    {TAY: 'Transfer accumulator to Y'},
    {TSX: 'Transfer stack pointer to X'},
    {TXA: 'Transfer X to accumulator'},
    {TXS: 'Transfer X to stack pointer'},
    {TYA: 'Transfer Y to accumulator'},
];

const exInstructions: Instruction[] = [
    {KIL: 'Crashes the microprocessor. Produces unexpected results'},
    {SLO: 'Arithmetic shift left + or with accumulator'},
    {RLA: 'rotate left + and (with accumulator)'},
    {SRE: 'logical shift right + exclusive or (with accumulator)'},
    {RRA: 'rotate right + add with carry'},
    {SAX: 'and accumulator with X + subtract immediate into X'},
    {LAX: 'load accumulator & X'},
    {DCP: 'decrement + compare (with accumulator)'},
    {DCM: 'decrement + compare (with accumulator)'},
    {ISC: 'increment + subtract with carry'},
    {INS: 'increment + subtract with carry'},
    {ANC: 'and (with accumulator) + set carry if negative'},
    {ALR: 'and (with accumulator) + logical shift right'},
    {ARR: 'and (with accumulator) + rotate right'},
    {XAA: 'transfer X to accumulator + and (with accumulator) '},
    {AXS: 'and X with accumulator into memory'},
    {OAL: 'or with accumulator $EE + and immediate + load accumulator & X'},
    {TAS: 'and accumulator and X into stack pointer + and with high-byte of address then store at address'},
    {SAY: 'and Y with high-byte + 1 of address then store at address'},
    {XAS: 'and X with high-byte + 1 of address then store at address'},
    {AXA: 'and X with accumulator & high-byte + 1 of address then store at address'},
];

const snippets: Snippet[] = [

    // addressing
    {label: 'Addressing: Absolute',            text: '$${1:0000}'},
    {label: 'Addressing: Absolute, X-indexed', text: '$${1:0000},X'},
    {label: 'Addressing: Absolute, Y-indexed', text: '$${1:0000},Y'},
    {label: 'Addressing: Immediate',           text: '#${1:$00}'},
    {label: 'Addressing: Indirect',            text: '$(${1:0000})'},
    {label: 'Addressing: X-indexed, Indirect', text: '$(${1:00},X)'},
    {label: 'Addressing: Indirect, Y-indexed', text: '$(${1:00}),Y'},
    {label: 'Addressing: Relative',            text: '$${1:00}'},
    {label: 'Addressing: Zeropage',            text: '$${1:00}'},
    {label: 'Addressing: Zeropage, X-indexed', text: '$${1:00},X'},
    {label: 'Addressing: Zeropage, Y-indexed', text: '$${1:00},Y'},
];


// definition ----------------------------------------------------------------------------------------------------------

const lang: Asm6502MonarchLanguage = {

    id:           'asm-6502',

    ignoreCase:    true,
    defaultToken: 'invalid',

    brackets: [
        { open: '{', close: '}', token: 'delimiter.curly' },
		{ open: '[', close: ']', token: 'delimiter.bracket' },
		{ open: '(', close: ')', token: 'delimiter.parenthesis' }
    ],


	symbols: /[=><!~?:&|+\-*/^%#]+/,
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

    instructions:   instructions.map((i) => Object.keys(i)[0]),
    exInstructions: exInstructions.map((i) => Object.keys(i)[0]),

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


// completion provider -------------------------------------------------------------------------------------------------

const Kind = monaco.languages.CompletionItemKind;

const suggestions: CompletionItem[] = [

    // instructions
    ...instructions.map((inst): CompletionItem => {
        const mnemonic = Object.keys(inst)[0];
        const desc     = inst[mnemonic];
        return {
            label:          mnemonic,
            kind:           Kind.Method,
            insertText:     mnemonic,
            // eslint-disable-next-line @typescript-eslint/no-explicit-any
            range:         (undefined as any),
            detail:         desc,
            documentation: {value: desc},
        };
    }),

    // snippets
    ...snippets.map((snip): CompletionItem => ({
        label:            snip.label,
        kind:             Kind.Snippet,
        insertText:       snip.text,
        insertTextRules:  CompletionItemInsertTextRule.InsertAsSnippet,
        // eslint-disable-next-line @typescript-eslint/no-explicit-any
        range:           (undefined as any),
})),
];

const completion: CompletionItemProvider = {
    provideCompletionItems(): CompletionList {
        return {suggestions};
    },
};


// export --------------------------------------------------------------------------------------------------------------

export default {

    id: lang.id,

    register(): void {
        monaco.languages.register({id: lang.id});
        monaco.languages.setMonarchTokensProvider(lang.id, lang);
        monaco.languages.registerCompletionItemProvider(lang.id, completion);
    },
};
