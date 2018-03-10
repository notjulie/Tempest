//
//  TempestViewRenderer.swift
//  iTempest
//
//  Created by Randy Rasmussen on 3/10/18.
//  Copyright © 2018 Randy Rasmussen. All rights reserved.
//

import Foundation
import MetalKit

class TempestViewRenderer {
   private let tempest : cTempest
   private var vectors : Array<TempestVector> = Array<TempestVector>()
   private var renderPipelineState: MTLRenderPipelineState?

   init(view:MTKView, tempest:cTempest) {
      // save parameters
      self.tempest = tempest
      
      // set the size of our vector array to some handy maximum
      var i = 0
      while (i < 2000) {
         vectors.append(TempestVector(startX: 0, startY: 0, endX: 0, endY: 0, r: 0, g: 0, b: 0))
         i = i + 1
      }
      
      // create our render pipeline
      let library = view.device!.newDefaultLibrary()!
      let vertexFunction = library.makeFunction(name: "tempestVertex")
      let fragmentFunction = library.makeFunction(name: "tempestFragment")
      let renderPipelineDescriptor = MTLRenderPipelineDescriptor()
      renderPipelineDescriptor.vertexFunction = vertexFunction
      renderPipelineDescriptor.fragmentFunction = fragmentFunction
      renderPipelineDescriptor.colorAttachments[0].pixelFormat = view.colorPixelFormat
      renderPipelineDescriptor.depthAttachmentPixelFormat = view.depthStencilPixelFormat
      do {
         renderPipelineState = try view.device!.makeRenderPipelineState(descriptor: renderPipelineDescriptor)
      } catch {
         Swift.print("Unable to compile render pipeline state")
         return
      }
   }
   
   public func render(renderEncoder : MTLRenderCommandEncoder) {
      // ask Tempest for a list of lines to draw
      var vectorCount : Int = vectors.count
      vectors.withUnsafeMutableBufferPointer { vectorBuffer in
         vectorCount = Int(cTempestGetVectors(tempest, vectorBuffer.baseAddress, Int32(vectorCount)))
      }
      
      // draw them
      if (vectorCount > 0)
      {
         // install our metal functions
         renderEncoder.setRenderPipelineState(renderPipelineState!)
         
         // create vector list on the GPU
         let dataSize = vectorCount * MemoryLayout.size(ofValue: vectors[0]);
         let vertexBuffer = renderEncoder.device.makeBuffer(bytes: vectors, length: dataSize, options: []);
         renderEncoder.setVertexBuffer(vertexBuffer, offset: 0, at: Int(0));
         
         // we have six vertices per vector = two triangles
         renderEncoder.drawPrimitives(type: .triangle, vertexStart: 0, vertexCount: vectorCount * 6, instanceCount: 1)
      }
   }
}
