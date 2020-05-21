//
//  MainMenu.ts
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 21 May 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//

import {
    app,
    shell,
    Menu,
    MenuItemConstructorOptions,
}               from 'electron';
import isDev    from 'electron-is-dev';


// helpers -------------------------------------------------------------------------------------------------------------

const isMac = process.platform === 'darwin';

type MiOptions = MenuItemConstructorOptions;
function _m(m: MiOptions): MiOptions;
function _m(m: MiOptions[]): MiOptions[];
function _m(m: MiOptions | MiOptions[]): MiOptions | MiOptions[] {
    return m;
}

const template: MiOptions[] = [];


// app menu ------------------------------------------------------------------------------------------------------------

if (isMac) {
    template.push({
        label: app.name,
        submenu: [
            { role: 'about' },
            { type: 'separator' },
            { role: 'services' },
            { type: 'separator' },
            { role: 'hide' },
            { role: 'hideOthers' },
            { role: 'unhide' },
            { type: 'separator' },
            { role: 'quit' }
        ],
    });
}


// file menu -----------------------------------------------------------------------------------------------------------

template.push({
    label: 'File',
    submenu: [
        {
            label:       'New',
            accelerator: 'CmdOrCtrl+N',
            click:       (): void => console.log('--debug menu: File > New'),
        },
        {
            label:       'New From Template...',
            accelerator: 'CmdOrCtrl+Shift+N',
            click:       (): void => console.log('--debug menu: File > New From Template'),
        },
        {
            label:       'Open',
            accelerator: 'CmdOrCtrl+O',
            click:       (): void => console.log('--debug menu: File > Open'),
        },
        { role: 'recentDocuments' },
        { type: 'separator' },
        {
            label:       'Save',
            accelerator: 'CmdOrCtrl+S',
            click:       (): void => console.log('--debug menu: File > Save'),
        },
        {
            label:       'Save As...',
            accelerator: 'CmdOrCtrl+Shift+S',
            click:       (): void => console.log('--debug menu: File > Save As...'),
        },
        {
            label:       'Save All',
            accelerator: 'CmdOrCtrl+Alt+S',
            click:       (): void => console.log('--debug menu: File > Save All'),
        },
        { type: 'separator' },
        ..._m(isMac ? [
            { role: 'close' }
        ] : [
            { role: 'quit' },
        ]),
    ],
});


// edit menu -----------------------------------------------------------------------------------------------------------

template.push({
    label: 'Edit',
    submenu: [
        { role: 'undo' },
        { role: 'redo' },
        { type: 'separator' },
        { role: 'cut' },
        { role: 'copy' },
        { role: 'paste' },
        ..._m(isMac ? [
            { role: 'delete' },
            { role: 'selectAll' },
            { type: 'separator' },
            {
                label: 'Speech',
                submenu: [
                    { role: 'startSpeaking' },
                    { role: 'stopSpeaking' },
                ]
            }
        ] : [
            { role: 'delete' },
            { type: 'separator' },
            { role: 'selectAll' }
        ]),
    ]
},);


// view menu -----------------------------------------------------------------------------------------------------------

template.push({
    label: 'View',
    submenu: [
        ..._m(isDev ? [
            { role: 'reload' },
            { role: 'forceReload' },
            { role: 'toggleDevTools' },
            { type: 'separator' },
        ] : []),
        { role: 'zoomIn' },
        { role: 'zoomOut' },
        { role: 'resetZoom' },
        { type: 'separator' },
        { role: 'togglefullscreen' }
    ],
});


// window menu ---------------------------------------------------------------------------------------------------------

template.push({
    label: 'Window',
    submenu: [
        { role: 'minimize' },
        { role: 'zoom' },
        ..._m(isMac ? [
            { type: 'separator' },
            { role: 'front' },
            { type: 'separator' },
            { role: 'window' }
        ] : [
            { role: 'close' }
        ])
    ]
});


// help menu -----------------------------------------------------------------------------------------------------------

template.push({
    role: 'help',
    submenu: [
        {
            label: 'Learn More',
            click: (): void => {
                shell.openExternal('https://github.com/rakeshta/6502-emulator/tree/master/platforms/electron/6502-app');
            },
        },
        ..._m(isMac ? [] : [
            { type: 'separator' },
            { role: 'about' },
        ]),
    ],
});



// build & export ------------------------------------------------------------------------------------------------------

export default Menu.buildFromTemplate(template);
