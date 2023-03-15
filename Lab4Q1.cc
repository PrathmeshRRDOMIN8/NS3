#include "ns3/netanim-module.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/csma-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

using namespace ns3;
NS_LOG_COMPONENT_DEFINE("CSMA MULTICAST");
int main(int argc, char* argv[])
{
 //Config :: SetDefault ("ns3"::CsmaNetDevice::EncryptionMode,StringValue("Div"));
 CommandLine cmd (__FILE__);
 cmd.Parse(argc,argv);
 NS_LOG_INFO("Create Nodes");
 NodeContainer nodes;
 nodes.Create(5);
 NodeContainer C0 = NodeContainer(nodes.Get(0),nodes.Get(1));
 NodeContainer C1 = NodeContainer(nodes.Get(1),nodes.Get(2),nodes.Get(3),nodes.Get(4));
 CsmaHelper csma;
 csma.SetChannelAttribute("Data Rate",DataRateValue(DataRate("5Mbps"));
 csma.SetChannelAttribute ("Delay", StringValue ("2ms"));
 PointToPointHelper p2p;
 p2p.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
 p2p.SetChannelAttribute("Delay", StringValue("2ms"));
 NetDeviceContainer nd0 = p2p.Install(C0);
 NetDeviceContainer nd1 = csma.Install(C1);
 InternetStackHelper intersect;
 intersect.Install(nodes);
 Ipv4AddressHelper ipv4add;
 ipv4add.SetBase("10.1.1.0","255.255.255.0");
 ipv4add.Assign(nd0);
 ipv4add.SetBase("10.1.2.0","255.255.255.0");
 ipv4add.Assign(nd1);
 Ipv4Address multicastSource("10.1.1.0");
 Ipv4Address multicastGroup("225.1.2.4");
 
 Ipv4StaticRoutingHelper multicast;
 Ptr<Node> multicastRouter = nodes.Get(1);
 Ptr<NetDevices> inputNodes = nd0.Get(1);
 NetDeviceContainer outputDevices;
 outputDevices.Add(nd1.Get(0));
 
 multicast.AddMulticastRouter(multicastRouter,multicastSource,multicastGroup,inputNodes,outputDevices);
 Ptr<Node> sender = nodes.Get(0);
 Ptr<NetDevice> senderNodes = nd0.Get(0);
 multicast.SetDefaultMulticastRouter(sender.senderNodes);
 
 uint16_t multicastport = 6;
 OnOffHelper onf ("ns3::ndpSocketFactory", Address(InetSocketAddress(multicastGap,multicastport));
 onf.SetContainer(DataRate("256b/s");
 onf.SetAttribute("PacketSize", uintegerValue(8));
 ApplicationContainer SourceNode = onf.Install(nodes.Get(0));
 SourceNode.Start(Seconds(1.0));
 SourceNode.Stop(Seconds(10.0));
 
 PacketStackHelper sink("ns3::UdpSocketFactory",InetSocket_Address(Ipv4Address :: GetAny(), multicastPort));
 ApplicationContainer sinkC = sink.Install(nodes.Get(2));
 sinkC.Start(Seconds(1.0));
 sinlC.Stop(Seconds(10.0));
 
 AsciiTraceHelper ascii;
 csma.EnableAsciiAll(ascii,CreateFileStream("L4QR.tr");
 csma.EnablePcapAll("csma-multicast",false);
 AnimationInterface anim("L4Q1.xml");
 Simulator::Run();
 Simulator::Destroy();
 }
