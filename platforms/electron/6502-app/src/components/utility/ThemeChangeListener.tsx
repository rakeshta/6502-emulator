//
//  ThemeChangeListener.tsx
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 28 May 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//

import React        from 'react';
import {
    remote,
    NativeTheme,
}                   from 'electron';


export interface Props {
    onChange?:  (nativeTheme: NativeTheme) => void;
}

export default class ThemeChangeListener extends React.PureComponent<Props> {

    // lifecycle -------------------------------------------------------------------------------------------------------

    public componentDidMount(): void {
        super.componentDidMount?.();

        // listen for theme changes
        remote.nativeTheme.addListener('updated', this._nativeTheme_onUpdated);
    }

    public componentWillUnmount(): void {
        super.componentWillUnmount?.();

        // stop listening to theme changes
        remote.nativeTheme.removeListener('updated', this._nativeTheme_onUpdated);
    }


    // events ----------------------------------------------------------------------------------------------------------

    private _nativeTheme_onUpdated = (): void => {
        this.props.onChange?.(remote.nativeTheme);
    };


    // render ----------------------------------------------------------------------------------------------------------

    public render(): React.ReactNode {
        return null;
    }
}
