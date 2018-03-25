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

   init(view:MTKView, shaderName:String) {
      // call the super
      super.init(view: view)
      
      // Render pipeline
      let library = view.device!.newDefaultLibrary()!
      let vertexFunction = library.makeFunction(name: "rectangleShaderVertex")
      let fragmentFunction = library.makeFunction(name: shaderName)
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

   func render(renderEncoder : MTLRenderCommandEncoder) {
      // install our metal functions
      renderEncoder.setRenderPipelineState(renderPipelineState!)
      
      // set our render parameters
      let renderVertexParameters =
         RectangleShaderVertexParameters(
            centerX: Float(-1 + 2 * self.frame.midX / view.frame.width),
            centerY: Float(1 - 2 * self.frame.midY / view.frame.height),
            xScale: Float(self.frame.width / view.frame.width),
            yScale: Float(self.frame.height / view.frame.height)
      )
      
      // turn them into metal buffers
      let renderVertexParametersBuffer: MTLBuffer = renderEncoder.device.makeBuffer(
         bytes: [renderVertexParameters],
         length: MemoryLayout.size(ofValue: renderVertexParameters),
         options: [])
      
      // encode our render commands
      renderEncoder.setVertexBuffer(renderVertexParametersBuffer, offset: 0, at: Int(RECTANGLE_SHADER_VERTEX_PARAMETERS_BUFFER_INDEX));
      renderEncoder.drawPrimitives(type: .triangleStrip, vertexStart: 0, vertexCount: 4, instanceCount: 1)
   }
   
}

