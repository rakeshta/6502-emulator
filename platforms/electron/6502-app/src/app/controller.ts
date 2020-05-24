//
//  controller.ts
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 22 May 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//

import {
    BrowserWindow,
    BrowserWindowConstructorOptions,
}                             from 'electron';
import isDev                  from 'electron-is-dev';

declare const MAIN_WINDOW_WEBPACK_ENTRY: string;


// helpers -------------------------------------------------------------------------------------------------------------

function _createAndShowWindow(
    type:    string,
    params:  Record<string, string> | null,
    options: BrowserWindowConstructorOptions,
): BrowserWindow {

    // create window
    const win = new BrowserWindow({
        show:                   false,
        webPreferences:       {
            enableRemoteModule: true,
            nodeIntegration:    true,
        },
        ...options,
    });

    // show when ready
    win.once('ready-to-show', () => win.show());

    // form URL & load page
    let suffix  = '#' + type;
    if (params) {
        const query = Object.entries(params).map(([key, val]) => `${key}=${encodeURIComponent(val)}`);
        if (query.length > 0) {
            suffix += '?' + query.join('&');
        }
    }
    console.log('Launching ', MAIN_WINDOW_WEBPACK_ENTRY + suffix);
    win.loadURL(MAIN_WINDOW_WEBPACK_ENTRY + suffix);

    // open dev-tools in dev-env
    if (isDev) {
        win.webContents.openDevTools();
    }

    return win;
}


// welcome window ------------------------------------------------------------------------------------------------------

let _welcomeWin: BrowserWindow | undefined;

const welcome = {

    show(): BrowserWindow {

        // focus existing if any
        if (_welcomeWin) {
            _welcomeWin.focus();
            return _welcomeWin;
        }

        // create new & show
        _welcomeWin = _createAndShowWindow('welcome', null, {
            width:       800,
            height:      600,
            resizable:   false,
            maximizable: false,
        });

        // hook listener to remove instance when closed
        _welcomeWin.once('close', () => _welcomeWin = undefined);

        return _welcomeWin;
    },

    close(): void {
        if (_welcomeWin) {
            _welcomeWin.close();
            _welcomeWin = undefined;
        }
    },
};


// document window -----------------------------------------------------------------------------------------------------

const _documentWindows: BrowserWindow[] = [];

function _createAndShowDocumentWindow(params:  Record<string, string>): BrowserWindow {

    // create & show window
    const win = _createAndShowWindow('document', params, {
        width:       1024,
        height:      768,
        minWidth:    800,
        minHeight:   600,
    });

    // add to list & remove when closed
    _documentWindows.push(win);
    win.once('close', () => {
        const index = _documentWindows.findIndex((w) => w === win);
        if (index >= 0) {
            _documentWindows.splice(index, 1);
        }
    });

    // close any welcome window when this one appears
    win.once('show', () => welcome.close());

    return win;
}

const document = {

    new(): BrowserWindow {
        return _createAndShowDocumentWindow({newFileName: 'Untitled'});
    },

    open(filePath: string): BrowserWindow {
        return _createAndShowDocumentWindow({filePath});
    },
};


// export --------------------------------------------------------------------------------------------------------------

export default {
    welcome,
    document,
};
