//
//  webpack.renderer.config.js
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 11 May 2020.
//  Copyright (c) 2020 Pixxa LLC. All rights reserved.
//

const rules                      = require('./webpack.rules');
const ForkTsCheckerWebpackPlugin = require('fork-ts-checker-webpack-plugin');


rules.push({
    test: /\.css$/,
    use: [{ loader: 'style-loader' }, { loader: 'css-loader' }],
});

module.exports = {
    module: {
        rules,
    },
    plugins: [
        new ForkTsCheckerWebpackPlugin({async: false}),
    ],
    resolve: {
        extensions: ['.js', '.ts', '.jsx', '.tsx', '.css']
    },
};
