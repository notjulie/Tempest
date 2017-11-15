//
//  GameViewController.m
//  iTempest
//
//  Created by Randy Rasmussen on 10/22/17.
//  Copyright © 2017 Randy Rasmussen. All rights reserved.
//

#import "GameViewController.h"
#import "Renderer.h"
#import "TempestView.h"

@implementation GameViewController
{
}

- (void)viewDidLoad
{
    // call the super
    [super viewDidLoad];
    
    // create our Tempest machine
    _tempest = cTempestCreate();

    // set our background
    self.view.layer.backgroundColor = [UIColor brownColor].CGColor;
    
    // create the TempestView
    //UIView *v = [[UIView alloc]initWithFrame:CGRectMake(0,0,200,200)];
    //[v setBackgroundColor:[UIColor redColor]];
    //[self.view addSubview:v];
    _tempestView = [[TempestView alloc]initWithFrame:CGRectMake(0,0,200,200)];
    [_tempestView setBackgroundColor:[UIColor redColor]];
    [self.view addSubview:_tempestView];

    // set up our function that gets called on screen updates
    CADisplayLink *displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(update)];
    displayLink.preferredFramesPerSecond = 60;
    [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
}

- (void)viewWillTransitionToSize:(CGSize)size
       withTransitionCoordinator:(id<UIViewControllerTransitionCoordinator>)coordinator
{
    
}


-(void)update
{
    // ask Tempest for a list of lines to draw
    cVectors vectors = cTempestGetVectors(_tempest);

//    TempestView *tempestView = (TempestView *)self.view;
    [_tempestView showVectors:vectors];
    
    // free up our vectors array
    cVectorsDispose(vectors);
}

@end
