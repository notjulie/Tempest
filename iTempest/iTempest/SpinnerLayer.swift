//
//  SpinnerLayer.swift
//  iTempest
//
//  Created by Randy Rasmussen on 1/7/18.
//  Copyright © 2018 Randy Rasmussen. All rights reserved.
//

import Foundation
import UIKit

class SpinnerLayer : CAMetalLayer
{
    private var isInitialized : Bool = false;
    private var mtlDevice : MTLDevice! = nil;
    private var pipelineState: MTLRenderPipelineState! = nil;
    private var commandQueue: MTLCommandQueue! = nil;
    private var timer: CADisplayLink! = nil;
    
    // stuff specific to the example
    private let vertexData:[Float] = [
        0.0, 1.0, 0.0,
        -1.0, -1.0, 0.0,
        1.0, -1.0, 0.0];
    private var vertexBuffer: MTLBuffer! = nil;

    func initialize() {
        // never mind if we're already initialized
        if (isInitialized) {
            return;
        }
        
        // create our display-link timer
        timer = CADisplayLink(target: self, selector: #selector(updateSpinner));
        timer.add(to: RunLoop.main, forMode: RunLoopMode.defaultRunLoopMode);
        
        // create our Metal device
        mtlDevice = MTLCreateSystemDefaultDevice();
        
        // create our command queue
        commandQueue = mtlDevice.makeCommandQueue();
        
        // we are the CAMetalLayer, so attach ourself to the device
        self.device = mtlDevice;
        self.pixelFormat = .bgra8Unorm // 3
        self.framebufferOnly = true    // 4
        self.frame = CGRect(x:0, y:0, width:100, height:100);
        
        // stuff specific to the example
        let dataSize = vertexData.count * MemoryLayout.size(ofValue: vertexData[0]);
        vertexBuffer = device!.makeBuffer(bytes: vertexData, length: dataSize, options: []);
        
        // example pipeline
        // 1
        let defaultLibrary = device!.newDefaultLibrary()!;
        let fragmentProgram = defaultLibrary.makeFunction(name: "basic_fragment");
        let vertexProgram = defaultLibrary.makeFunction(name: "basic_vertex");
        
        // 2
        let pipelineStateDescriptor = MTLRenderPipelineDescriptor();
        pipelineStateDescriptor.vertexFunction = vertexProgram;
        pipelineStateDescriptor.fragmentFunction = fragmentProgram;
        pipelineStateDescriptor.colorAttachments[0].pixelFormat = .bgra8Unorm;
        
        // 3
        pipelineState = try! device!.makeRenderPipelineState(descriptor: pipelineStateDescriptor);
        
        // mark ourself initialized
        isInitialized = true;
    }
    
    func render() {
        guard let drawable = self.nextDrawable() else { return }
        let renderPassDescriptor = MTLRenderPassDescriptor()
        renderPassDescriptor.colorAttachments[0].texture = drawable.texture
        renderPassDescriptor.colorAttachments[0].loadAction = .clear
        renderPassDescriptor.colorAttachments[0].clearColor = MTLClearColor(red: 0.0, green: 104.0/255.0, blue: 5.0/255.0, alpha: 1.0)
    
        let commandBuffer = commandQueue.makeCommandBuffer()
        
        let renderEncoder = commandBuffer.makeRenderCommandEncoder(descriptor: renderPassDescriptor)
        renderEncoder.setRenderPipelineState(pipelineState)
        renderEncoder.setVertexBuffer(vertexBuffer, offset: 0, at: 0)
        renderEncoder.drawPrimitives(type: .triangle, vertexStart: 0, vertexCount: 3, instanceCount: 1)
        renderEncoder.endEncoding()
        
        commandBuffer.present(drawable)
        commandBuffer.commit()
        
    }
    
    func updateSpinner() {
        autoreleasepool {
            self.render()
        }
    }
}

