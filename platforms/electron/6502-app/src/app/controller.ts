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

import { background }         from '../styles/theme';

declare const MAIN_WINDOW_WEBPACK_ENTRY: string;


// helpers -------------------------------------------------------------------------------------------------------------

function _createAndShowWindow(
    type:    string,
    params:  Record<string, string> | null,
    options: BrowserWindowConstructorOptions,
): BrowserWindow {

    // create window
    const win = new BrowserWindow({
        darkTheme:             true,
        backgroundColor:       background.chrome,
        show:                  false,
        webPreferences:       {
            nodeIntegration:   true,
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

const welcome = {

    show(): BrowserWindow {
        return _createAndShowWindow('welcome', null, {
            width:       800,
            height:      600,
            resizable:   false,
            maximizable: false,
        });
    },
};


// document window -----------------------------------------------------------------------------------------------------

const document = {

    open(filePath: string): BrowserWindow {
        return _createAndShowWindow('document', {filePath}, {
            width:       1024,
            height:      768,
            minWidth:    800,
            minHeight:   600,
        });
    },
};


// export --------------------------------------------------------------------------------------------------------------

export default {
    welcome,
    document,
};
