using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using DNP3.Adapter;
using DNP3.Interface;

namespace DERTestSet.dnp3
{
    public class DEROutstation : ICommandHandler, ITimeWriteHandler
    {
        IOutstation m_outstation;

        public DEROutstation()
        {
        }

        /*********************************************************************************************/

        public void setOutstation(IOutstation outstation)
        {
            m_outstation = outstation;
        }

        public CommandStatus Supports(ControlRelayOutputBlock command, uint index)
        {
            throw new NotImplementedException();
        }

        /*********************************************************************************************/

        public CommandStatus Supports(AnalogOutputInt32 command, uint index)
        {
            throw new NotImplementedException();
        }

        /*********************************************************************************************/

        public CommandStatus Supports(AnalogOutputInt16 command, uint index)
        {
            throw new NotImplementedException();
        }

        /*********************************************************************************************/

        public CommandStatus Supports(AnalogOutputFloat32 command, uint index)
        {
            throw new NotImplementedException();
        }

        /*********************************************************************************************/

        public CommandStatus Supports(AnalogOutputDouble64 command, uint index)
        {
            throw new NotImplementedException();
        }

        /*********************************************************************************************/

        public CommandStatus Perform(ControlRelayOutputBlock command, uint index)
        {
            throw new NotImplementedException();
        }

        /*********************************************************************************************/

        public CommandStatus Perform(AnalogOutputInt32 command, uint index)
        {
            throw new NotImplementedException();
        }

        /*********************************************************************************************/

        public CommandStatus Perform(AnalogOutputInt16 command, uint index)
        {
            throw new NotImplementedException();
        }

        /*********************************************************************************************/

        public CommandStatus Perform(AnalogOutputFloat32 command, uint index)
        {
            throw new NotImplementedException();
        }

        /*********************************************************************************************/

        public CommandStatus Perform(AnalogOutputDouble64 command, uint index)
        {
            throw new NotImplementedException();
        }

        /*********************************************************************************************/

        public void WriteAbsoluteTime(long millisecSinceEpoch)
        {
            Console.Out.WriteLine("time sync: " + millisecSinceEpoch);
        }
    }
}
