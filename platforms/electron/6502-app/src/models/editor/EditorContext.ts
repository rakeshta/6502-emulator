//
//  EditorContext.ts
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 20 May 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//


export default class EditorContext {

    private _fileName:   string;
    private _filePath?:  string;

    private _contents:   string;


    // constructor -----------------------------------------------------------------------------------------------------

    private constructor(fileName: string, contents: string) {
        this._fileName = fileName;
        this._contents = contents;
    }


    // factory methods -------------------------------------------------------------------------------------------------

    public static create(params: {fileName: string}): EditorContext {
        return new EditorContext(params.fileName, '; untitled file\n');
    }


    // accessors -------------------------------------------------------------------------------------------------------
}
