//
//  TempestLayer.swift
//  iTempest
//
//  Created by Randy Rasmussen on 1/13/18.
//  Copyright © 2018 Randy Rasmussen. All rights reserved.
//

import Foundation
import UIKit

class TempestLayer : CAMetalLayer
{
   private var isInitialized : Bool = false;
   private var mtlDevice : MTLDevice! = nil;
   private var pipelineState: MTLRenderPipelineState! = nil;
   private var commandQueue: MTLCommandQueue! = nil;
   private var timer: CADisplayLink! = nil;
   private var rotation : Float = 0;
   private var vertexBuffer: MTLBuffer! = nil;
   private var renderParametersBuffer: MTLBuffer! = nil;
   
   private func initialize() {
      // never mind if we're already initialized
      if (isInitialized) {
         return;
      }
      
      // create our Metal device
      mtlDevice = MTLCreateSystemDefaultDevice();
      
      // create our command queue
      commandQueue = mtlDevice.makeCommandQueue();
      
      // we are the CAMetalLayer, so attach ourself to the device
      self.device = mtlDevice;
      self.pixelFormat = .bgra8Unorm // 3
      self.framebufferOnly = true    // 4
      self.backgroundColor = UIColor.black.cgColor;
      
      // example pipeline
      // 1
      let defaultLibrary = device!.newDefaultLibrary()!;
      let fragmentProgram = defaultLibrary.makeFunction(name: "tempestFragment");
      let vertexProgram = defaultLibrary.makeFunction(name: "tempestVertex");
      
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
   
   public func showVectors(vectors:cVectors)
   {
      initialize();
      
      guard let drawable = self.nextDrawable() else { return }

      let commandBuffer = commandQueue.makeCommandBuffer();

      let renderPassDescriptor = MTLRenderPassDescriptor()
      renderPassDescriptor.colorAttachments[0].texture = drawable.texture
      renderPassDescriptor.colorAttachments[0].loadAction = .clear;
      renderPassDescriptor.colorAttachments[0].clearColor = MTLClearColor(red: 0.0, green: 0.0, blue: 0.0/255.0, alpha: 0.0)
      let parallelRenderEncoder = commandBuffer.makeParallelRenderCommandEncoder(descriptor: renderPassDescriptor)

      // create our vertex array
      while (true)
      {
         var startX : Int16 = 0;
         var endX : Int16 = 0;
         var startY : Int16 = 0;
         var endY : Int16 = 0;
         var r : UInt8 = 0;
         var g : UInt8 = 0;
         var b : UInt8 = 0;

         // pop the vectors into an array
         var vectorData : [TempestVector] = [];
         while (0 != cVectorsGetNext(vectors, &startX, &startY, &endX, &endY, &r, &g, &b))
         {
            vectorData += [
               TempestVector(startX: startX, startY: startY, endX:endX, endY:endY, r:r, g:g, b:b)
            ];
         }

         if (vectorData.count == 0)
         {
            break;
         }
         
         // create vector list on the GPU
         let dataSize = vectorData.count * MemoryLayout.size(ofValue: vectorData[0]);
         vertexBuffer = device!.makeBuffer(bytes: vectorData, length: dataSize, options: []);

         //let renderEncoder = commandBuffer.makeRenderCommandEncoder(descriptor: renderPassDescriptor)
         let renderEncoder = parallelRenderEncoder.makeRenderCommandEncoder();
         renderEncoder.setRenderPipelineState(pipelineState)
         renderEncoder.setVertexBuffer(vertexBuffer, offset: 0, at: Int(0));
         
         // we have six vertices per vector = two triangles
         renderEncoder.drawPrimitives(type: .triangle, vertexStart: 0, vertexCount: vectorData.count * 6, instanceCount: 1)
         renderEncoder.endEncoding()
      }
      
      parallelRenderEncoder.endEncoding();
      commandBuffer.present(drawable);
      commandBuffer.commit();
   }
}

