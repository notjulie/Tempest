using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace TempestWpf
{
   /// <summary>
   /// Represents log data reported by the MathBox
   /// </summary>
   public class MathBoxLog
   {
      private List<MathBoxLogEntry> entries = new List<MathBoxLogEntry>();

      /// <summary>
      /// Creates an instance of MathBoxLog from the given xml string
      /// </summary>
      /// <param name="xml">the XML</param>
      /// <returns>the created instance</returns>
      public static MathBoxLog FromXML(string xml)
      {
         // create the result object that we're building
         MathBoxLog result = new MathBoxLog();

         // load the documents
         XmlDocument doc = new XmlDocument();
         doc.LoadXml(xml);

         // parse all the entries
         for (int i=0; i<doc.DocumentElement.ChildNodes.Count; ++i)
         {
            XmlElement entryElement = doc.DocumentElement.ChildNodes[i] as XmlElement;
            if (entryElement == null)
               continue;
            if (entryElement.Name != "Entry")
               continue;
            result.entries.Add(MathBoxLogEntry.FromElement(entryElement));
         }

         return result;
      }
   }
}
