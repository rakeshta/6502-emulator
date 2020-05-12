//
//  webpack.renderer.config.js
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 11 May 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//

/* eslint-disable @typescript-eslint/no-var-requires */
const ForkTsCheckerWebpackPlugin = require('fork-ts-checker-webpack-plugin');
const rules                      = require('./webpack.rules');

const { toSass }                 = require('./src/style/theme');


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
        new ForkTsCheckerWebpackPlugin({async: false}),
    ],
    resolve: {
        extensions: ['.js', '.ts', '.jsx', '.tsx', '.css']
    },
};
