//
//  Editor.tsx
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 15 May 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//

import React        from 'react';
import * as monaco  from 'monaco-editor';

import dasm         from 'dasm';
import hexy         from 'hexy';
import throttle     from 'lodash/throttle';

import theme        from '../style/theme';

import testProg     from '../components/editor/test-prog';

import './assembly6502';

import './Editor.scss';


// editor theme --------------------------------------------------------------------------------------------------------

monaco.editor.defineTheme('custom', {
    base:    'vs-dark',
    inherit:  true,
    rules:   [
        {token: 'comment',     foreground: theme.editor.comment,     fontStyle: 'italic'},
        {token: 'instruction', foreground: theme.editor.instruction},
    ],
    colors:  {
        'editor.background': theme.background.primary,
    },
});


// class Editor --------------------------------------------------------------------------------------------------------

export interface Props {
    className?:  string;
}

export default class Editor extends React.PureComponent<Props> {

    private _containerRef         = React.createRef<HTMLDivElement>();

    private _model?:                monaco.editor.ITextModel;
    private _editor?:               monaco.editor.IStandaloneCodeEditor;

    private _containerWidthDelta  = 0;
    private _containerHeightDelta = 0;


    // lifecycle -------------------------------------------------------------------------------------------------------

    public componentDidMount(): void {

        // container should be mounted by now
        const container = this._containerRef.current;
        if (!container) { return; }

        // compute initial size delta of container to window. this is used to recompute correct size
        // when window is resized
        this._containerWidthDelta  = document.body.clientWidth  - container.clientWidth;
        this._containerHeightDelta = document.body.clientHeight - container.clientHeight;

        // add window resize listener to allow editor to resize
        window.addEventListener('resize', this._onWindowResize);

        // mount editor
        this._model  = monaco.editor.createModel(testProg, 'assembly6502')
        this._model?.onDidChangeContent(this._model_onDidChangeContent);
        this._editor = monaco.editor.create(container, {
            // eslint-disable-next-line @typescript-eslint/no-non-null-assertion
            model:                 this._model!,
            theme:                'custom',
            scrollBeyondLastLine:  false,
        });

        // DEBUG
        this._assemble(testProg);
    }

    public componentWillUnmount(): void {

        // remove resize listener
        window.removeEventListener('resize', this._onWindowResize);

        // dispose editor & model
        if (this._editor) { this._editor.dispose(); this._model  = undefined; }
        if (this._model)  { this._model.dispose();  this._model  = undefined; }
    }


    // helpers ---------------------------------------------------------------------------------------------------------

    private _assemble(code: string): void {

        console.log('--debug assembling')
        const start = Date.now();
        const res   = dasm(code, {
            format: 2,
        });
        console.group('--debug assembled in ', (Date.now() - start) / 1000.0 + 's');
        console.log(res);
        if (res.success) {
            console.log('--data:');
            console.log(hexy.hexy(Buffer.from(res.data), {
                width:   8,
                format: 'twos',
                caps:   'upper',
            }));
        }
        console.groupEnd();
    }

    private _assembleThrottled = throttle(this._assemble, 1000);


    // events ----------------------------------------------------------------------------------------------------------

    private _onWindowResize = (): void => {

        const container  = this._containerRef.current;
        if (container && this._editor) {
            const width  = document.body.clientWidth  - this._containerWidthDelta;
            const height = document.body.clientHeight - this._containerHeightDelta;
            this._editor.layout({width, height});
        }
    }

    private _model_onDidChangeContent = (): void => {
        const value = this._model?.getValue() || '';
        this._assembleThrottled(value);
    }


    // render ----------------------------------------------------------------------------------------------------------

    public render(): React.ReactNode {
        return (
            <div className='tab-editor py-1'>
                <div ref={this._containerRef} className='monaco-container'/>
            </div>
        );
    }
}
