//
//  WelcomeWindow.tsx
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 21 May 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//

import React      from 'react';
import { remote } from 'electron';

import './WelcomeWindow.scss';


export default class WelcomeWindow extends React.PureComponent {

    // events ----------------------------------------------------------------------------------------------------------

    private _closeButton_onClick = (): void => {
        remote.getCurrentWindow().close();
    };


    // render ----------------------------------------------------------------------------------------------------------

    public render(): React.ReactNode {
        return (
            <div className='window welcome'>
                {this._renderSplash()}
                {this._renderRecents()}
            </div>
        );
    }

    private _renderSplash(): React.ReactNode {
        return (
            <div className='split splash'>

                <div className='top-bar'>
                    <button className='btn btn-close' onClick={this._closeButton_onClick}>×</button>
                </div>

                <h2 className='m-auto'>6502 Emulator</h2>
            </div>
        );
    }

    private _renderRecents(): React.ReactNode {
        return (
            <div className='split recents p-2'>
                <p>Recent items will be listed here</p>
            </div>
        );
    }
}
