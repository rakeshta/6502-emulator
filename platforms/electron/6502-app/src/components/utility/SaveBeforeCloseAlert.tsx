//
//  SaveBeforeCloseAlert.tsx
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 29 May 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//

import React    from 'react';
import {
    remote,
    BrowserWindow,
}               from 'electron';

import logger   from '../../utils/logger';


const log = logger(__filename);


export interface Props {
    fileName?:   string | (() => string | undefined);
    onSave?:    (completion: () => void) => unknown;
}

export default class SaveBeforeCloseAlert extends React.Component<Props> {

    // accessors -------------------------------------------------------------------------------------------------------

    private get browserWindow(): BrowserWindow {
        return remote.getCurrentWindow();
    }


    // lifecycle -------------------------------------------------------------------------------------------------------

    public componentDidMount(): void {
        super.componentDidMount?.();

        // hook before unload listener
        window.addEventListener('beforeunload', this._onBeforeUnload);
    }

    public componentWillUnmount(): void {
        super.componentWillUnmount?.();

        // unhook before unload listener
        // window.removeEventListener('beforeunload', this._onBeforeUnload);
    }

    public shouldComponentUpdate(): boolean {
        return false;
    }


    // helpers ---------------------------------------------------------------------------------------------------------

    private _closeBrowserWindow(): void {
        window.removeEventListener('beforeunload', this._onBeforeUnload);
        this.browserWindow.close();
    }


    // events ----------------------------------------------------------------------------------------------------------

    private _onBeforeUnload = async (event: BeforeUnloadEvent): Promise<void> => {

        // do not stop unload if no unsaved changes
        const win = this.browserWindow;
        if (!win.isDocumentEdited()) {
            return;
        }

        // error out if file name cannot be determined or if save callback not provided
        const {fileName, onSave} = this.props;
        const fileNameU = typeof fileName === 'function' ? fileName() : fileName;
        if (!fileNameU || !onSave) {
            log.error('Unable to determine current file name or save callback not provided. Aborting.');
            return;
        }

        // prevent unload
        event.returnValue = false;

        // show alert to confirm user choice
        const res = await remote.dialog.showMessageBox(win, {
            type:     'warning',
            buttons: ['&Save', '&Cancel', '&Don\'t Save'],
            cancelId:  1,
            message:  `Do you want to save the changes you made to ${fileNameU}?`,
            detail:   'Your changes will be lost if you don\'t save them.',
            normalizeAccessKeys: true,
        });

        switch (res.response) {
            // save
            case 0: // save
                onSave(() => this._closeBrowserWindow());
                break;

            case 1: // cancel
            default:
                break;

            case 2: // don't save
                this._closeBrowserWindow();
                break;
        }
    };


    // render ----------------------------------------------------------------------------------------------------------

    public render(): React.ReactNode {
        return null;
    }
}
