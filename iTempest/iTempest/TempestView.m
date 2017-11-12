//
//  TempestView.m
//  iTempest
//
//  Created by Randy Rasmussen on 10/29/17.
//  Copyright © 2017 Randy Rasmussen. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "TempestView.h"

@implementation TempestView
{
}

- (instancetype)init
{
    self = [super init];
    if (self) {
        self.layer.backgroundColor = [UIColor blueColor].CGColor;
    }
    return self;
}

@end

