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

    private _editorRef        = React.createRef<Editor>();

    private _fileName?:         string;
    private _filePath?:         string;

    private _diskFileWatcher?:  fs.FSWatcher;


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

        // skip if no change
        if (value === this._filePath) { return; }

        // stop listening to changes on old file
        if (this._diskFileWatcher) {
            this._diskFileWatcher.close();
            this._diskFileWatcher = undefined;
        }

        // save file path & file name
        this._filePath = value;
        this.fileName  = value && path.basename(value);

        // start listening to changes on new file
        if (value) {
            this._diskFileWatcher = fs.watch(value, this._diskFile_onChange);
        }

        // update represented file
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

    public componentWillUnmount(): void {
        super.componentWillUnmount?.();

        // stop listening to file events
        if (this._diskFileWatcher) {
            this._diskFileWatcher.close();
            this._diskFileWatcher = undefined;
        }
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
        console.log('--debug will write');
        await writeFile(filePath, text, 'utf8');
        console.log('--debug did write');

        // update file path & mark editor not dirty
        this.filePath = filePath;
        this.isEdited = false;
    }


    // file events -----------------------------------------------------------------------------------------------------

    private _diskFile_onChange = (eventType: string, fileName: string): void => {
        const isFocused = this.browserWindow.isFocused();
        console.log('--debug file event', {eventType, fileName, isFocused});
    };


    // ui events -------------------------------------------------------------------------------------------------------

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
