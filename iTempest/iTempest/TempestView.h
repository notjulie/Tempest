//
//  TempestView.h
//  iTempest
//
//  Created by Randy Rasmussen on 10/29/17.
//  Copyright © 2017 Randy Rasmussen. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface TempestView : UIView
@property NSMutableArray *lines;
@property NSMutableArray *paths;
@property bool initialized;

-(void) showVectors:(cVectors) vectors;

@end

