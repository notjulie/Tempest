//
//  TempestView.swift
//  iTempest
//
//  Created by Randy Rasmussen on 11/15/17.
//  Copyright © 2017 Randy Rasmussen. All rights reserved.
//

import Foundation
import UIKit
import MetalKit

class TempestView : MTKView {
   private var initialized : Bool = false;
   private var commandQueue: MTLCommandQueue?
   private var renderPipelineState: MTLRenderPipelineState?
   private var depthStencilState: MTLDepthStencilState?
   private var vectorData : [TempestVector] = [];
   private var tempest : cTempest = 0;

   func initialize(tempest : cTempest) {
      if (initialized)
      {
         return;
      }
      
      // save parameters
      self.tempest = tempest
      
      // Device
      device = MTLCreateSystemDefaultDevice()
      
      // View
      clearColor = MTLClearColorMake(0.0, 0.0, 0.0, 1)
      colorPixelFormat = .bgra8Unorm
      depthStencilPixelFormat = .depth32Float
      
      // Command queue
      commandQueue = device!.makeCommandQueue()
      
      // Render pipeline
      let library = device!.newDefaultLibrary()!
      let vertexFunction = library.makeFunction(name: "tempestVertex")
      let fragmentFunction = library.makeFunction(name: "tempestFragment")
      let renderPipelineDescriptor = MTLRenderPipelineDescriptor()
      renderPipelineDescriptor.vertexFunction = vertexFunction
      renderPipelineDescriptor.fragmentFunction = fragmentFunction
      renderPipelineDescriptor.colorAttachments[0].pixelFormat = colorPixelFormat
      renderPipelineDescriptor.depthAttachmentPixelFormat = depthStencilPixelFormat
      do {
         renderPipelineState = try device!.makeRenderPipelineState(descriptor: renderPipelineDescriptor)
      } catch {
         Swift.print("Unable to compile render pipeline state")
         return
      }
      
      // Depth stencil
      let depthSencilDescriptor = MTLDepthStencilDescriptor()
      depthSencilDescriptor.depthCompareFunction = .less
      depthSencilDescriptor.isDepthWriteEnabled = true
      depthStencilState = device!.makeDepthStencilState(descriptor: depthSencilDescriptor)
      
      initialized = true;
   }

   override func draw(_ dirtyRect: CGRect) {
      // ask Tempest for a list of lines to draw
      let vectors : cVectors = cTempestGetVectors(tempest);

      // pop the vectors into an array
      var startX : Int16 = 0;
      var endX : Int16 = 0;
      var startY : Int16 = 0;
      var endY : Int16 = 0;
      var r : UInt8 = 0;
      var g : UInt8 = 0;
      var b : UInt8 = 0;
      vectorData = [];
      while (0 != cVectorsGetNext(vectors, &startX, &startY, &endX, &endY, &r, &g, &b))
      {
         vectorData += [
            TempestVector(startX: startX, startY: startY, endX:endX, endY:endY, r:r, g:g, b:b)
         ];
      }

      // free up our vectors object
      cVectorsDispose(vectors);

      let commandBuffer = commandQueue!.makeCommandBuffer()
      let renderPassDescriptor = currentRenderPassDescriptor!
      let renderEncoder = commandBuffer.makeRenderCommandEncoder(descriptor: renderPassDescriptor)
      
      renderEncoder.setFrontFacing(.counterClockwise)
      renderEncoder.setDepthStencilState(depthStencilState)
      renderEncoder.setRenderPipelineState(renderPipelineState!)
      
      if (vectorData.count > 0)
      {
         // create vector list on the GPU
         let dataSize = vectorData.count * MemoryLayout.size(ofValue: vectorData[0]);
         let vertexBuffer = device!.makeBuffer(bytes: vectorData, length: dataSize, options: []);
         renderEncoder.setVertexBuffer(vertexBuffer, offset: 0, at: Int(0));
         
         // we have six vertices per vector = two triangles
         renderEncoder.drawPrimitives(type: .triangle, vertexStart: 0, vertexCount: vectorData.count * 6, instanceCount: 1)
      }
      
      renderEncoder.endEncoding()
      let drawable = currentDrawable!
      commandBuffer.present(drawable)
      commandBuffer.commit()
   }
}

