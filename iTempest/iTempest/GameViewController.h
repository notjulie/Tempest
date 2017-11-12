//
//  GameViewController.h
//  iTempest
//
//  Created by Randy Rasmussen on 10/22/17.
//  Copyright © 2017 Randy Rasmussen. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>
#import "Renderer.h"
#import "cTempest.h"

// Our iOS view controller
@interface GameViewController : UIViewController
@property CAShapeLayer *line;
@property int cycle;
@property cTempest *tempest;
@end
