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
   private var tempest : cTempest = 0
   private var vectors : Array<TempestVector> = Array<TempestVector>()

   func initialize(tempest : cTempest) {
      if (initialized)
      {
         return;
      }
      
      // save parameters
      self.tempest = tempest
      
      // set the size of our vector array to some handy maximum
      var i = 0
      while (i < 2000) {
         vectors.append(TempestVector(startX: 0, startY: 0, endX: 0, endY: 0, r: 0, g: 0, b: 0))
         i = i + 1
      }
      
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
      var vectorCount : Int = vectors.count
      vectors.withUnsafeMutableBufferPointer { vectorBuffer in
         vectorCount = Int(cTempestGetVectors(tempest, vectorBuffer.baseAddress, Int32(vectorCount)))
      }

      let commandBuffer = commandQueue!.makeCommandBuffer()
      let renderPassDescriptor = currentRenderPassDescriptor!
      let renderEncoder = commandBuffer.makeRenderCommandEncoder(descriptor: renderPassDescriptor)
      
      renderEncoder.setFrontFacing(.counterClockwise)
      renderEncoder.setDepthStencilState(depthStencilState)
      renderEncoder.setRenderPipelineState(renderPipelineState!)
      
      if (vectorCount > 0)
      {
         // create vector list on the GPU
         let dataSize = vectorCount * MemoryLayout.size(ofValue: vectors[0]);
         let vertexBuffer = device!.makeBuffer(bytes: vectors, length: dataSize, options: []);
         renderEncoder.setVertexBuffer(vertexBuffer, offset: 0, at: Int(0));
         
         // we have six vertices per vector = two triangles
         renderEncoder.drawPrimitives(type: .triangle, vertexStart: 0, vertexCount: vectorCount * 6, instanceCount: 1)
      }
      
      renderEncoder.endEncoding()
      let drawable = currentDrawable!
      commandBuffer.present(drawable)
      commandBuffer.commit()
   }
}

