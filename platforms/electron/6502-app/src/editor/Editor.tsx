//
//  Editor.tsx
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 15 May 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//

import React        from 'react';
import * as monaco  from 'monaco-editor';

import './Editor.scss';


export interface Props {
    className?:  string;
}

export default class Editor extends React.PureComponent<Props> {

    private _monacoContainerRef = React.createRef<HTMLDivElement>();


    // lifecycle -------------------------------------------------------------------------------------------------------

    public componentDidMount(): void {

        // mount editor
        // eslint-disable-next-line @typescript-eslint/no-non-null-assertion
        monaco.editor.create(this._monacoContainerRef.current!, {
            value:    'console.log("Hello World");',
            language: 'typescript',
            theme:    'vs-dark',
        });
    }


    // render ----------------------------------------------------------------------------------------------------------

    public render(): React.ReactNode {
        return (
            <div className='tab-editor'>
                <div ref={this._monacoContainerRef} className='monaco-container'/>
            </div>
        );
    }
}
