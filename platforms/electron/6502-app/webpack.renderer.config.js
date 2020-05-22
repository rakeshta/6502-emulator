//
//  webpack.renderer.config.js
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 11 May 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//

/* eslint-disable @typescript-eslint/no-var-requires */
const ForkTsCheckerWebpackPlugin = require('fork-ts-checker-webpack-plugin');
const MonacoWebpackPlugin        = require('monaco-editor-webpack-plugin');
const rules                      = require('./webpack.rules');

const { toSass }                 = require('./src/styles/theme');


module.exports = {
    module: {
        rules: rules.concat([
            {
                test: /\.(png|jpg|jpeg|gif|tiff|woff|woff2|eot|ttf|svg)$/,
                use: ['url-loader'],
            },
            {
                test: /\.s?css$/,
                use: [
                    'style-loader',
                    'css-loader',
                    {
                        loader: 'sass-loader',
                        options: {
                            prependData: toSass(),
                        },
                    }
                ],
            },
        ]),
    },
    plugins: [
        new ForkTsCheckerWebpackPlugin({async: false, eslint: true}),
        new MonacoWebpackPlugin({
            languages: [],
            features:  [
                'accessibilityHelp', 'bracketMatching', 'caretOperations', 'clipboard', 'codeAction', /* 'codelens', */
                /* 'colorDetector', */ 'comment', 'contextmenu', 'coreCommands', 'cursorUndo', 'dnd', 'find', 'folding',
                'fontZoom', 'format', 'gotoError', 'gotoLine', 'gotoSymbol', 'hover', /* 'iPadShowKeyboard', */
                'inPlaceReplace', 'inspectTokens', 'linesOperations', 'links', 'multicursor', 'parameterHints',
                'quickCommand', 'quickOutline', 'referenceSearch', 'rename', 'smartSelect', 'snippets', 'suggest',
                'toggleHighContrast', 'toggleTabFocusMode', 'transpose', 'wordHighlighter', 'wordOperations',
                'wordPartOperations'
            ],
        }),
    ],
    resolve: {
        extensions: ['.js', '.ts', '.jsx', '.tsx', '.css']
    },
};
