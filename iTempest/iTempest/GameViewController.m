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
    // call the super
    [super viewDidLoad];
    
    // create our Tempest machine
    _tempest = cTempestCreate();

    // set our background
    self.view.layer.backgroundColor = [UIColor blackColor].CGColor;
    
    // set up our function that gets called on screen updates
    CADisplayLink *displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(update)];
    displayLink.preferredFramesPerSecond = 60;
    [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    
    // create our arrays of screen objects
    _lines = [NSMutableArray array];
    _paths = [NSMutableArray array];
    for (int i=0; i<1000; ++i)
    {
        UIBezierPath *path = [UIBezierPath bezierPath];
        [_paths addObject:path];

        CAShapeLayer *line = [CAShapeLayer layer];
        line.strokeColor = [UIColor darkGrayColor].CGColor;
        [self.view.layer addSublayer:line];
        [_lines addObject:line];
    }
}

-(void)update
{
    int16_t startX, endX, startY, endY;
    uint8_t r, g, b;
    int i;
    
    // ask Tempest for a list of lines to draw
    cVectors vectors = cTempestGetVectors(_tempest);
    
    float scale = 64 * 1024;

    // draw lines
    i = 0;
    while (cVectorsGetNext(vectors, &startX, &startY, &endX, &endY, &r, &g, &b))
    {
        if (i >= _lines.count)
            break;
        
        int width = self.view.frame.size.width;
        int height = self.view.frame.size.height;
        float size = width < height ? width : height;

        
        UIBezierPath *path = [_paths objectAtIndex:i];
        [path removeAllPoints];
        [path moveToPoint:CGPointMake(size * (0.5 + startX/scale), size * (0.5 - startY/scale))];
        [path addLineToPoint:CGPointMake(size * (0.5 + endX/scale), size * (0.5 - endY/scale))];

        CAShapeLayer *layer = (CAShapeLayer *)[_lines objectAtIndex:i];
        layer.path = path.CGPath;
        layer.opacity = 1;
        layer.strokeColor = [UIColor colorWithRed:r/255.0 green:g/255.0 blue:b/255.0 alpha:1].CGColor;

        ++i;
    }

    // hide the rest of the lines in the array
    while (i < _lines.count)
    {
        CAShapeLayer *layer = (CAShapeLayer *)[_lines objectAtIndex:i];
        layer.opacity = 0;
        ++i;
    }
    
    // free up our vectors array
    cVectorsDispose(vectors);
}

@end
