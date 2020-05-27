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
import {
    remote,
    BrowserWindow,
}                     from 'electron';

import IpcListener    from '../../components/utility/IpcListener';
import Editor         from '../../components/editor/Editor';

import config         from '../../config';

import './DocumentWindow.scss';


const readFile  = promisify(fs.readFile);
const writeFile = promisify(fs.writeFile);


// class DocumentWindow ------------------------------------------------------------------------------------------------

export interface Props {
    filePath?:    string;
    newFileName?: string;
}

export default class DocumentWindow extends React.PureComponent<Props, {}> {

    private _editorRef  = React.createRef<Editor>();


    private _fileName?:   string;
    private _filePath?:   string;


    // accessors -------------------------------------------------------------------------------------------------------

    private get browserWindow(): BrowserWindow {
        return remote.getCurrentWindow();
    }

    private get fileName(): string | undefined {
        return this._fileName;
    }

    private set fileName(value: string | undefined) {
        if (value === this._fileName) { return; }
        this._fileName = value;
        if (value) {
            this.browserWindow.setTitle(value);
        }
    }

    private get filePath(): string | undefined {
        return this._filePath;
    }

    private set filePath(value: string | undefined) {
        if (value === this._filePath) { return; }
        this._filePath = value;
        this.fileName  = value && path.basename(value);
        if (value) {
            this.browserWindow.setRepresentedFilename(value);
        }
    }

    private get isEdited(): boolean {
        return this.browserWindow.isDocumentEdited();
    }

    private set isEdited(value: boolean) {
        this.browserWindow.setDocumentEdited(value);
    }


    // lifecycle -------------------------------------------------------------------------------------------------------

    public componentDidMount(): void {
        super.componentDidMount?.();

        // initialize file name path
        const {newFileName, filePath} = this.props;
        if (filePath) {
            this.filePath = filePath;
        }
        else if (newFileName) {
            this.fileName = newFileName;
        }

        // load contents
        this._load();
    }


    // load / save -----------------------------------------------------------------------------------------------------

    private async _load(): Promise<void> {

        // load file if given
        const filePath = this.filePath;
        if (filePath) {
            const contents = await readFile(filePath, 'utf8');
            this._editorRef.current?.setText(contents);
        }
    }

    private async _save(): Promise<void> {

        // show save dialog if no file path
        let filePath = this.filePath;
        if (!filePath) {

            // show save dialog, abort if cancelled
            const res = await remote.dialog.showSaveDialog(this.browserWindow, {
                defaultPath:  this._fileName,
                filters:      config.file.filters,
                properties: ['createDirectory', 'showOverwriteConfirmation'],
            });
            if (!res.filePath) {
                return;
            }

            // continue with file path
            filePath = res.filePath;
        }

        // save file
        const text = this._editorRef.current?.getText() ?? '';
        await writeFile(filePath, text, 'utf8');

        // update file path & mark editor not dirty
        this.filePath = filePath;
        this.isEdited = false;
    }


    // events ----------------------------------------------------------------------------------------------------------

    private _ipc_onSave = (): void => {
        this._save();
    };

    private _editor_onChange = (): void => {
        this.isEdited = true;
    };


    // render ----------------------------------------------------------------------------------------------------------

    public render(): React.ReactNode {
        return (
            <div className='window document'>
                <IpcListener channel='menu.save' listener={this._ipc_onSave}/>
                <Editor ref={this._editorRef} className='editor' onChange={this._editor_onChange}/>
            </div>
        );
    }
}
