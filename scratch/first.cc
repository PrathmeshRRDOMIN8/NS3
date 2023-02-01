/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"

// Default Network Topology
//
//       10.1.1.0
// n0 -------------- n1
//    point-to-point
//
 
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");

int
main (int argc, char *argv[])
{
  CommandLine cmd (__FILE__);
  cmd.Parse (argc, argv);
  
  Time::SetResolution (Time::NS);
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

  NodeContainer nodes;
  //nodes.Create (2);
  nodes.Create (3);
  
  //PointToPointHelper pointToPoint;
  //pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  //pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
  PointToPointHelper P2P1;
  PointToPointHelper P2P2;
  P2P1.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  P2P1.SetChannelAttribute ("Delay", StringValue ("2ms"));
  P2P2.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  P2P2.SetChannelAttribute ("Delay", StringValue ("2ms"));
  //create another pointtopoint an device.
  //nodes.get(0), nodes.get(1)
  //NetDeviceContainer devices;
  //devices = pointToPoint.Install (nodes);
  NetDeviceContainer devices1, devices2;
  devices1 = P2P1.Install (nodes.Get (0), nodes.Get (1));
  devices2 = P2P2.Install (nodes.Get (1), nodes.Get (2));
  InternetStackHelper stack;
  stack.Install (nodes);
  // create another address. //WAIT __ CHECK AGAIN
  Ipv4AddressHelper address1, address2;
  address1.SetBase ("10.1.1.0", "255.255.255.0");
  address2.SetBase ("10.1.2.0", "255.255.255.0");

  //Ipv4InterfaceContainer interfaces = address.Assign (devices);
  //create another interface
  Ipv4InterfaceContainer interfaces1 = address1.Assign (devices1);
  Ipv4InterfaceContainer interfaces2 = address2.Assign (devices2);
  UdpEchoServerHelper echoServer (9);
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
  ApplicationContainer serverApps = echoServer.Install (nodes.Get (2)); //will become 2
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  UdpEchoClientHelper echoClient (interfaces2.GetAddress (1), 9);  // interfaces2 last address which is the end node.
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (0.5)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps = echoClient.Install (nodes.Get (0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
