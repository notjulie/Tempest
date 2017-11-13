//
//  TempestView.m
//  iTempest
//
//  Created by Randy Rasmussen on 10/29/17.
//  Copyright © 2017 Randy Rasmussen. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "cTempest.h"
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

-(void) showVectors:(cVectors) vectors
{
    if (!_initialized)
    {
        // create our arrays of screen objects
        _lines = [NSMutableArray array];
        _paths = [NSMutableArray array];
        for (int i=0; i<1000; ++i)
        {
            UIBezierPath *path = [UIBezierPath bezierPath];
            [_paths addObject:path];
            
            CAShapeLayer *line = [CAShapeLayer layer];
            line.strokeColor = [UIColor darkGrayColor].CGColor;
            [self.layer addSublayer:line];
            [_lines addObject:line];
        }
        
        _initialized = true;
    }
    
    int16_t startX, endX, startY, endY;
    uint8_t r, g, b;
    int i;
    
    float scale = 64 * 1024;
    
    // draw lines
    i = 0;
    while (cVectorsGetNext(vectors, &startX, &startY, &endX, &endY, &r, &g, &b))
    {
        if (i >= _lines.count)
            break;
        
        int width = self.frame.size.width;
        int height = self.frame.size.height;
        float size = width < height ? width : height;

        // get the current path and clear it
        UIBezierPath *path = [_paths objectAtIndex:i];
        [path removeAllPoints];

        // tempest sends a zero length vector to indicate a dot; deal with that
        if (startX==endX && startY==endY)
        {
            [path moveToPoint:CGPointMake(size * (0.5 + startX/scale) - 0.5, size * (0.5 - startY/scale))];
            [path addLineToPoint:CGPointMake(size * (0.5 + endX/scale) + 0.5, size * (0.5 - endY/scale))];
        }
        else
        {
            [path moveToPoint:CGPointMake(size * (0.5 + startX/scale), size * (0.5 - startY/scale))];
            [path addLineToPoint:CGPointMake(size * (0.5 + endX/scale), size * (0.5 - endY/scale))];
        }
        
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
}

@end

