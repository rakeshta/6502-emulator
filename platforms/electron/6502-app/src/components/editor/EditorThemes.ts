//
//  EditorThemes.ts
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 28 May 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//

import * as monaco    from 'monaco-editor';
import {light, dark}  from '../../styles/theme';


type IStandaloneThemeData = monaco.editor.IStandaloneThemeData;
type BuiltinTheme         = monaco.editor.BuiltinTheme;
type AppTheme             = typeof light | typeof dark;


function _themeFrom(base: BuiltinTheme, theme: AppTheme): IStandaloneThemeData {
    return {
        base,
        inherit:  true,
        rules:   [
            {token: 'comment',     foreground: theme.editor.comment,     fontStyle: 'italic'},
            {token: 'instruction', foreground: theme.editor.instruction},
        ],
        colors:  {
            'editor.background': theme.background.primary,
        },
    };
}


const EditorThemes = {
    '6502-light': _themeFrom('vs',      light),
    '6502-dark':  _themeFrom('vs-dark', dark),
};
export default EditorThemes;

export type EditorThemeName = keyof typeof EditorThemes;
