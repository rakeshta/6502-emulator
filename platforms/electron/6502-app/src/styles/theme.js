//
//  theme.js
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 12 May 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//
/* eslint-disable @typescript-eslint/explicit-function-return-type */
// NOTE: This file has to be JS as it is included from webpack.renderer.config.js


// theme ---------------------------------------------------------------------------------------------------------------

const theme = {
    light: {
        color: {
            primary:      '#A30B37',
        },
        background: {
            chrome:       '#c1c8cc',
            primary:      '#f7fcff',
            secondary:    '#f0f0f7',
        },
        foreground: {
            primary:      '#202020',
            secondary:    '#606060',
            muted:        '#cccccc',
            ghost:        '#e0e0e0',
        },
        editor: {
            comment:      '#5C6370',
            instruction:  '#D19A66',
        },
    },
    dark: {
        color: {
            primary:      '#A30B37',
        },
        background: {
            chrome:       '#0F1A20',
            primary:      '#1E282D',
            secondary:    '#272C33',
        },
        foreground: {
            primary:      '#FAF8FC',
            secondary:    '#aaaaaa',
            muted:        '#606060',
            ghost:        '#303030',
        },
        editor: {
            comment:      '#5C6370',
            instruction:  '#D19A66',
        },
    },
};


// other style settings ------------------------------------------------------------------------------------------------

const style = {
    editor: {
        fontFamily: 'Victor Mono',
        fontWeight: '500',
        fontSize:    14,
    },
};


// sass ----------------------------------------------------------------------------------------------------------------

/*
 * Function to create SASS variable definitions from the theme.
 *
 * @returns a string that can be injected into all SASS files using the `sass-loader` webpack plugin.
 * @see `/webpack.renderer.config.js`
 */
const toSass = () => {
    let str = '';

    function extract(path, obj) {
        for (const [key, val] of Object.entries(obj)) {
            const pathEx = path + '-' + key;
            if (typeof val === 'object') {
                extract(pathEx, val);
            }
            else {
                str += `$${pathEx}: ${val};\n`;
            }
        }
    }
    extract('theme', theme);

    return str;
};


// export --------------------------------------------------------------------------------------------------------------

module.exports = {
    ...theme,
    style,
    toSass,
};
