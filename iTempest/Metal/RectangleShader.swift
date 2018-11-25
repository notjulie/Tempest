//
//  RectangleShader.swift
//  iTempest
//
//  Created by Randy Rasmussen on 3/25/18.
//  Copyright © 2018 Randy Rasmussen. All rights reserved.
//

import Foundation
import MetalKit

class RectangleShader : MetalRenderer {
   private var renderPipelineState: MTLRenderPipelineState?
   private var newFragmentBuffers: [Int:Any] = [:]
   private var fragmentBuffers: [Int:MTLBuffer] = [:]

   init(view:MTKView, shaderName:String) {
      // call the super
      super.init(view: view)
      
      // Render pipeline
      let library = view.device!.makeDefaultLibrary()!
      let vertexFunction = library.makeFunction(name: "rectangleShaderVertex")
      let fragmentFunction = library.makeFunction(name: shaderName)
      let renderPipelineDescriptor = MTLRenderPipelineDescriptor()
      renderPipelineDescriptor.vertexFunction = vertexFunction
      renderPipelineDescriptor.fragmentFunction = fragmentFunction
      renderPipelineDescriptor.colorAttachments[0].pixelFormat = view.colorPixelFormat
      renderPipelineDescriptor.depthAttachmentPixelFormat = view.depthStencilPixelFormat
      
      // for now just always allow alpha blending
      renderPipelineDescriptor.colorAttachments[0].isBlendingEnabled = true
      renderPipelineDescriptor.colorAttachments[0].rgbBlendOperation = .add
      renderPipelineDescriptor.colorAttachments[0].alphaBlendOperation = .add
      renderPipelineDescriptor.colorAttachments[0].destinationRGBBlendFactor = .oneMinusSourceAlpha
      renderPipelineDescriptor.colorAttachments[0].destinationAlphaBlendFactor = .oneMinusSourceAlpha

      do {
         renderPipelineState = try view.device!.makeRenderPipelineState(descriptor: renderPipelineDescriptor)
      } catch {
         Swift.print("Unable to compile render pipeline state")
         return
      }
   }

   override func render(renderEncoder : MTLRenderCommandEncoder) {
      // install our metal functions
      renderEncoder.setRenderPipelineState(renderPipelineState!)
      
      // set our render parameters
      let frame = self.getFrame()
      let renderVertexParameters =
         RectangleShaderVertexParameters(
            centerX: Float(-1 + 2 * frame.midX / view.frame.width),
            centerY: Float(1 - 2 * frame.midY / view.frame.height),
            xScale: Float(frame.width / view.frame.width),
            yScale: Float(frame.height / view.frame.height)
      )
      
      // turn them into a metal buffer
      let renderVertexParametersBuffer: MTLBuffer = renderEncoder.device.makeBuffer(
         bytes: [renderVertexParameters],
         length: MemoryLayout.size(ofValue: renderVertexParameters),
         options: [])!
      renderEncoder.setVertexBuffer(renderVertexParametersBuffer, offset: 0, index: Int(RECTANGLE_SHADER_VERTEX_PARAMETERS_BUFFER_INDEX));

      // create new fragment buffers
      for (index,buffer) in self.newFragmentBuffers {
         let renderVertexParametersBuffer: MTLBuffer = renderEncoder.device.makeBuffer(
            bytes: [buffer],
            length: MemoryLayout.size(ofValue: buffer),
            options: [])!
         setFragmentBuffer(index: index, buffer: renderVertexParametersBuffer)
      }
      
      // add the subclass's fragment buffers
      for (index,buffer) in self.fragmentBuffers {
         renderEncoder.setFragmentBuffer(buffer, offset: 0, index: index)
      }
      
      // encode our render commands
      renderEncoder.drawPrimitives(type: .triangleStrip, vertexStart: 0, vertexCount: 4, instanceCount: 1)
   }

   public func setFragmentBuffer(index:Int, buffer:MTLBuffer) {
      fragmentBuffers[index] = buffer
   }
   
   public func addFragmentBuffer(index:Int, o:Any) {
      newFragmentBuffers[index] = o
   }
}

