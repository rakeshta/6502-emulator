//
//  logger.ts
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 28 May 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//

import path  from 'path';


// eslint-disable-next-line @typescript-eslint/no-explicit-any
export type LogFunction = (message: string, ...params: any[]) => void;

export interface Logger {
    debug:   LogFunction;
    verbose: LogFunction;
    info:    LogFunction;
    warn:    LogFunction;
    error:   LogFunction;
}

const styles = {
    debug:   'color:#9437FF',
    verbose: 'color:#919191',
    info:    '',
    warn:    '',
    error:   '',
};

export default function logger(name: string): Logger {
    const prefix = '%c' + path.basename(name) + ': ';
    return {
        debug:   (message, ...params): void => console.log  (prefix + message, styles.debug,   ...params),
        verbose: (message, ...params): void => console.log  (prefix + message, styles.verbose, ...params),
        info:    (message, ...params): void => console.info (prefix + message, styles.info,    ...params),
        warn:    (message, ...params): void => console.warn (prefix + message, styles.warn,    ...params),
        error:   (message, ...params): void => console.error(prefix + message, styles.error,   ...params),
    };
}
