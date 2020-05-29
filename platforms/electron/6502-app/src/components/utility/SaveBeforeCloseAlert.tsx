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



export default class SaveBeforeCloseAlert extends React.Component {

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
        window.removeEventListener('beforeunload', this._onBeforeUnload);
    }

    public shouldComponentUpdate(): boolean {
        return false;
    }


    // events ----------------------------------------------------------------------------------------------------------

    private _onBeforeUnload = async (event: BeforeUnloadEvent): Promise<void> => {

        // do not stop unload if no unsaved changes
        const win = this.browserWindow;
        if (!win.isDocumentEdited()) {
            return;
        }

        // prevent unload
        console.log('--debug before unload');
        event.returnValue = false;

        // show alert to confirm user choice
        const res = await remote.dialog.showMessageBox(win, {
            type:     'warning',
            buttons: ['Save', 'Cancel', 'Don\'t Save'],
            cancelId:  1,
            message:  'Do you wish to save the changes you made to the document \'DOCUMENT NAME HERE\'?',
            detail:   'Your changes will be lost if you don\'t save them.',
        });
        console.log('--debug res', res);
    };


    // render ----------------------------------------------------------------------------------------------------------

    public render(): React.ReactNode {
        return null;
    }
}
