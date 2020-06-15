//
//  WorkerResponse.ts
//  6502-emulator
//
//  Created by Rakesh Ayyaswami on 15 Jun 2020.
//  Copyright (c) 2020 Raptor Soft. All rights reserved.
//

export default interface WorkerResponse<T> {
    id:        number;
    result:    T;
}
