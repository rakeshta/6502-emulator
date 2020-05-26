//
//  Editor.tsx
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 15 May 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//

import React        from 'react';
import * as monaco  from 'monaco-editor';
import classes      from 'classnames';


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
        this._model  = monaco.editor.createModel('');
        this._model?.onDidChangeContent(this._model_onDidChangeContent);
        this._editor = monaco.editor.create(container, {
            // eslint-disable-next-line @typescript-eslint/no-non-null-assertion
            model:                 this._model!,
            theme:                'custom',
            scrollBeyondLastLine:  false,
        });

        // add some space above the first line
        // magic code copied from: https://github.com/Microsoft/monaco-editor/issues/1333
        this._editor?.changeViewZones((accessor) => accessor.addZone({
            afterLineNumber: 0,
            heightInLines:   0.5,
            domNode:         document.createElement('div'),
        }));

        // focus on the editor
        this._editor?.focus();
    }

    public componentWillUnmount(): void {

        // remove resize listener
        window.removeEventListener('resize', this._onWindowResize);

        // dispose editor & model
        if (this._editor) { this._editor.dispose(); this._model  = undefined; }
        if (this._model)  { this._model.dispose();  this._model  = undefined; }
    }


    // events ----------------------------------------------------------------------------------------------------------

    private _onWindowResize = (): void => {

        const container  = this._containerRef.current;
        if (container && this._editor) {
            const width  = document.body.clientWidth  - this._containerWidthDelta;
            const height = document.body.clientHeight - this._containerHeightDelta;
            this._editor.layout({width, height});
        }
    };

    private _model_onDidChangeContent = (): void => {
        const value = this._model?.getValue() || '';
        console.log('--debug value', value);
    };


    // render ----------------------------------------------------------------------------------------------------------

    public render(): React.ReactNode {
        const {className} = this.props;
        return (
            <div ref={this._containerRef} className={classes('monaco-container', className)}/>
        );
    }
}
