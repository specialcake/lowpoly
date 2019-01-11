//
//  TimeStamp.swift
//  Polyworld
//
//  Created by 沈心逸 on 2019/1/11.
//  Copyright © 2019年 Sxy. All rights reserved.
//

import Foundation

extension Date {
    
    /// 获取当前 秒级 时间戳 - 10位
    var timeStamp : Int {
        let timeInterval: TimeInterval = self.timeIntervalSince1970
        let timeStamp = Int(timeInterval)
        return timeStamp
    }
    
    /// 获取当前 毫秒级 时间戳 - 13位
    var milliStamp : Int64 {
        let timeInterval: TimeInterval = self.timeIntervalSince1970
        let millisecond = Int64(round(timeInterval*1000))
        return millisecond
    }
}
