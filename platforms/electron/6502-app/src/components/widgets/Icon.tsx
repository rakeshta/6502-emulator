//
//  Icon.tsx
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 25 May 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//

import React     from 'react';
import {
    FontAwesomeIcon,
    FontAwesomeIconProps,
}                from '@fortawesome/react-fontawesome';
import {
    IconProp,
}                from '@fortawesome/fontawesome-svg-core';
import {
    faTimes,
}                from '@fortawesome/free-solid-svg-icons';


// Icon type ----------------------------------------------------------------------------------------------------------

export type Type = 'close';

const _icon = (type?: Type): IconProp | undefined => {
    switch (type) {
        case 'close':  return faTimes;
        default:       return ['far', 'question-square'];
    }
};


// component Icon ------------------------------------------------------------------------------------------------------

export interface Props extends Omit<FontAwesomeIconProps, 'icon'> {
    type?: Type;
}


export default React.memo(function Icon(props: Props) {

    const {type, ...rest} = props;

    // render nothing if no icon
    const icon = _icon(type);
    if (!icon) {
        return null;
    }

    // render fontawesome icon
    return <FontAwesomeIcon className={'icon ' + type} {...rest} icon={icon}/>;
});
