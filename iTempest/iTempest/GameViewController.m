//
//  GameViewController.m
//  iTempest
//
//  Created by Randy Rasmussen on 10/22/17.
//  Copyright © 2017 Randy Rasmussen. All rights reserved.
//

#import "GameViewController.h"
#import "Renderer.h"

@implementation GameViewController
{
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    _tempest = cTempestCreate();

    self.view.layer.backgroundColor = [UIColor blueColor].CGColor;
    
    CALayer *redLayer = [CALayer layer];
    redLayer.backgroundColor = [UIColor redColor].CGColor;
    redLayer.frame = CGRectMake(50.0, 50.0, 100.0, 100.0);
    
    [self.view.layer addSublayer:redLayer];

    CALayer *redLayer2 = [CALayer layer];
    redLayer2.backgroundColor = [UIColor redColor].CGColor;
    redLayer2.frame = CGRectMake(150.0, 150.0, 100.0, 100.0);
    
    [self.view.layer addSublayer:redLayer2];
    
    UIBezierPath *path = [UIBezierPath bezierPath];
    [path moveToPoint:CGPointMake(50, 50)];
    [path addLineToPoint:CGPointMake(150,150)];
    
    _line = [CAShapeLayer layer];
    _line.path = path.CGPath;
    _line.strokeColor = [UIColor darkGrayColor].CGColor;
    [self.view.layer addSublayer:_line];
    
    CADisplayLink *displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(update)];
    displayLink.preferredFramesPerSecond = 60;
    [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
}

-(void)update
{
    _cycle = _cycle + 1;
    _cycle = _cycle % 100;

    UIBezierPath *path = [UIBezierPath bezierPath];
    [path moveToPoint:CGPointMake(50 + _cycle, 50)];
    [path addLineToPoint:CGPointMake(150,150)];
    _line.path = path.CGPath;

}

@end
