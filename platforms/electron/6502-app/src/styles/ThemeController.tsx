//
//  ThemeController.tsx
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 25 May 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//

import React        from 'react';
import { remote }   from 'electron';

import theme        from './theme';


export default class ThemeController extends React.PureComponent {


    // lifecycle -------------------------------------------------------------------------------------------------------

    public componentDidMount(): void {
        super.componentDidMount?.();

        // initialize theme
        this._updateTheme();

        // listen for theme changes
        remote.nativeTheme.addListener('updated', this._updateTheme);
    }

    public componentWillUnmount(): void {
        super.componentWillUnmount?.();

        // stop listening to theme changes
        remote.nativeTheme.removeListener('updated', this._updateTheme);
    }


    // theme -----------------------------------------------------------------------------------------------------------

    private _updateTheme = (): void => {

        // determine theme
        const paletteType = remote.nativeTheme.shouldUseDarkColors ? 'dark' : 'light';

        // set theme on BrowserWindow, HTML & MaterialUI
        remote.getCurrentWindow().setBackgroundColor(theme[paletteType].background.secondary);
        document.documentElement.setAttribute('data-theme', paletteType);
        this.setState({paletteType});
    };


    // render ----------------------------------------------------------------------------------------------------------

    public render(): React.ReactNode {
        return null;
    }
}
