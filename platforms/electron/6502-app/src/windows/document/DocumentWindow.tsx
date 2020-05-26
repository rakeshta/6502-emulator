//
//  DocumentWindow.tsx
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 21 May 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//

import React          from 'react';

import fs             from 'fs';
import path           from 'path';
import { promisify }  from 'util';

import IpcListener    from '../../components/utility/IpcListener';

import Editor         from '../../components/editor/Editor';

import './DocumentWindow.scss';


const readFile = promisify(fs.readFile);


// class DocumentWindow ------------------------------------------------------------------------------------------------

export interface Props {
    filePath?:    string;
    newFileName?: string;
}

interface State {
    filePath?:    string;
    fileName?:    string;
}

export default class DocumentWindow extends React.PureComponent<Props, State> {

    public state: State = {};

    private _editorRef  = React.createRef<Editor>();


    // lifecycle -------------------------------------------------------------------------------------------------------

    public componentDidMount(): void {
        super.componentDidMount?.();
        this._reload();
    }


    // load ------------------------------------------------------------------------------------------------------------

    private async _reload(): Promise<void> {

        // load file if given
        const filePath     = this.state.filePath || this.props.filePath;
        if (filePath) {

            // TODO: should we save this to state?
            const fileName = path.basename(filePath);
            this.setState({fileName, filePath});

            // load contents into editor
            const contents = await readFile(filePath, 'utf8');
            this._editorRef.current?.setText(contents);
        }

        // set filename if new file
        else if (this.props.newFileName) {
            const fileName = this.props.newFileName;
            this.setState({fileName});
        }
    }


    // events ----------------------------------------------------------------------------------------------------------

    private _ipc_onSave = (): void => {
        console.log('--debug save');
    };


    // render ----------------------------------------------------------------------------------------------------------

    public render(): React.ReactNode {
        return (
            <div className='window document'>
                <IpcListener channel='menu.save' listener={this._ipc_onSave}/>
                <Editor ref={this._editorRef} className='editor'/>
            </div>
        );
    }
}
