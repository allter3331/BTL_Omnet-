/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package generatefattree;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author Dell
 */
public class FatTreeGraph extends Graph {

    public static final int CORE = 1;
    public static final int AGG = 2;
    public static final int EDGE = 3;
    private final int numServers;
    private final int numPodSwitches;
    private final int numCores;
    private final int k;
    //   private Address[] address;

    private List<Integer> switches;
    private List<Integer> hosts;
    final double CORE_PADDING ;
    final double AGG_PADDING ;
    final double PC_PADDING;
    final double WIDTH = 3000;
    final double HEIGHT = 1200;

    public FatTreeGraph(int k) {
        
        if (k % 2 == 1) {
            throw new IllegalArgumentException("K must be even");
        }
        if (k > 256) {
            throw new IllegalArgumentException("K must less than 256");
        }

        this.k = k;
        this.numServers = k * k * k / 4;
        this.numPodSwitches = k * k;
        this.numCores = k * k / 4;
        
        CORE_PADDING = WIDTH / numCores ;
        AGG_PADDING = WIDTH / ( numPodSwitches / 2 );
        PC_PADDING = WIDTH / numServers ;
        
        this.V = numServers + numPodSwitches + numCores;
        this.E = 0;
        adj = (List<Integer>[]) new List[V];
        for (int v = 0; v < V; v++) {
            adj[v] = new ArrayList<Integer>();
        }

        String nedStatement = new String("simple SimpleHost\n"
                + "{\n"
                
                + "    parameters:\n"
                + "\n"
                + "        @display(\"i=block/routing\");\n"
                + "    gates:\n"
                + "         inout gate[ ];\n"
                + "}\n"
                +"simple SimpleSwitch\n"
                + "{\n"                
                + "    parameters:\n"
                + "\n"
                + "        @display(\"i=block/routing\");\n"
                + "    gates:\n"
                + "         inout gate[ ];\n"
                + "}\n"
                + "network Network6\n"
                + "{\n"
                +"\tparameters:\n" +
                "        int k = "+ k +";\n"+
                "        int EXB_SIZE = default(5);\n" +
                "        double SIMULATOR_TIME = default(1); // Simulation time = 1s\n" +
                "        double OPERATION_CYCLE_TIME_SWITCH = default(0.00000001); \n" +
                "        double CREDIT_DELAY = default(0.000000001); \n" +
                "        double MSG_GEN_INTERVAL = default(0.0001);\n"                
                + "\n \t@display(\"bgb=" + WIDTH + ", " + HEIGHT + "\");\n"
                + "    \n"
                + "      \n"+
                "types:\n" +
                "        channel Channel extends ned.DatarateChannel\n" +
                "        {\n" +
                "            datarate = 1Gbps;\n" +
                "        }"
                + "    submodules:\n"
                + "        \n\t\t");
        
//                + "        node[" + this.V + "]: SimpleHost {\n"
//                + "            parameters:                \n"
//                + "                @display(\"i=,chocolate3\");\n"
//                + "        }\n"
        int tempCore = 0;
        int tempAgg = 0;
        int tempEdge = 0;
        int tempServer = 0;
        int paddingLeft = 100;
        
        for(int i = 0; i < V; i++){
            if( isCoreSwitch(i) ){
                double x;
                if(tempCore == 0) {
                    x = (paddingLeft + tempCore * CORE_PADDING );
                }else{
                    x = (tempCore * CORE_PADDING );
                }
              nedStatement = nedStatement +  "node"+i+" : SimpleSwitch {\n" +
                "            parameters:\n" +
                "                @display(\"p="+
                     x
                      +", "+ 50 +"\");\n" +
                "        }\n\t\t";
              tempCore++;
              
        
            }else if( isAggSwitch(i) ){
                double x;
                if(tempCore == 0) {
                    x = (paddingLeft +tempAgg * AGG_PADDING);
                }else{
                    x = tempAgg * AGG_PADDING;
                }
                nedStatement = nedStatement +  "node"+i+" : SimpleSwitch {\n" +
                "            parameters:\n" +
                "                @display(\"p="+
                     (paddingLeft +tempAgg * AGG_PADDING )
                      +", "+ 450 +"\");\n" +
                "        }\n\t\t";
                tempAgg++;
                
            }else if( isEdgeSwitch(i) ) {
                double x;
                if(tempCore == 0) {
                    x = paddingLeft +tempEdge * AGG_PADDING;
                }else{
                    x = tempEdge * AGG_PADDING;
                }
                nedStatement = nedStatement +  "node"+i+" : SimpleSwitch {\n" +
                "            parameters:\n" +
                "                @display(\"p="+
                     (paddingLeft +tempEdge * AGG_PADDING )
                      +", "+ 650 +"\");\n" +
                "        }\n\t\t";
                tempEdge++;
                
            }else if( isServer(i) ){
                double x;
                if(tempCore == 0) {
                    x = paddingLeft +tempServer * PC_PADDING;
                }else{
                    x = tempServer * PC_PADDING;
                }
                nedStatement = nedStatement +  "node"+i+" : SimpleHost {\n" +
                "            parameters:\n" +
                "                @display(\"p="+
                     (paddingLeft +tempServer * PC_PADDING )
                      +", "+ 1050 +"\");\n" +
                "        }\n\t\t";
                tempServer++;
               
            }
        }
        
        nedStatement = nedStatement + " \n"        
                + "    connections:\n";

        // each pod has k^2/4 servers and k switches
        int numEachPod = k * k / 4 + k;
        for (int p = 0; p < k; p++) {
            int offset = numEachPod * p;

            // between server and edge
            for (int e = 0; e < k / 2; e++) {
                int edgeSwitch = offset + k * k / 4 + e;
                for (int s = 0; s < k / 2; s++) {
                    int server = offset + e * k / 2 + s;
                    addEdge(edgeSwitch, server);
                    nedStatement = nedStatement + "\t\tnode" + edgeSwitch + ".gate++ <-->  Channel  <--> node" + server + ".gate++;\n";

                }
            }

            // between agg and edge
            for (int e = 0; e < k / 2; e++) {
                int edgeSwitch = offset + k * k / 4 + e;
                for (int a = k / 2; a < k; a++) {
                    int aggSwitch = offset + k * k / 4 + a;
                    addEdge(edgeSwitch, aggSwitch);
                    nedStatement = nedStatement + "\t\tnode" + edgeSwitch + ".gate++ <--> Channel  <--> node" + aggSwitch + ".gate++;\n";

                }
            }

            // between agg and core
            for (int a = 0; a < k / 2; a++) {
                int aggSwitch = offset + k * k / 4 + k / 2 + a;
                for (int c = 0; c < k / 2; c++) {
                    int coreSwitch = a * k / 2 + c + numPodSwitches + numServers;
                    addEdge(aggSwitch, coreSwitch);
                    nedStatement = nedStatement + "\t\tnode" + aggSwitch + ".gate++ <--> Channel  <--> node" + coreSwitch + ".gate++;\n";

                }
            }

        }
        nedStatement = nedStatement + "\n}";
        writeFile(nedStatement);

        // buildAddress();
    }
    
    private boolean isCoreSwitch(int i){
        if(i >= k*k*k / 4 + k*k) return true;
        else return false;
    }
    
    private boolean isAggSwitch(int i){
        if(i >= k * k * k / 4 + k*k) return false;

        int numEachPod = k * k / 4 + k;

        if((i % numEachPod) >= (k*k / 4 + k / 2)) return true;
            else return false;
    }
    
    private boolean isEdgeSwitch(int i){
        if(i >= k * k * k / 4 + k*k) return false;
        int numEachPod = k * k / 4 + k;
        if((i % numEachPod) >= k*k / 4 && (i % numEachPod) < (k*k / 4 + k/2)) return true;
            else return false;
    }
    
    private boolean isServer(int i){
        if(i >= k * k * k / 4 + k*k) return false;
        int numEachPod = k * k / 4 + k;

        if((i % numEachPod) < k*k / 4 ) return true;
            else return false;
    }

    @Override
    public List<Integer> hosts() {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public List<Integer> switches() {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public boolean isHostVertex(int v) {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    @Override
    public boolean isSwitchVertex(int v) {
        throw new UnsupportedOperationException("Not supported yet."); //To change body of generated methods, choose Tools | Templates.
    }

    private static void writeFile(String data) {
        OutputStream os = null;
        try {

            File nedFile = new File("D:\\test_part5.txt ");
            if (!nedFile.exists()) {
                try {
                    nedFile.createNewFile();
                } catch (IOException ex) {
                    Logger.getLogger(FatTreeGraph.class.getName()).log(Level.SEVERE, null, ex);
                }
            }
            os = new FileOutputStream(nedFile);
            os.write(data.getBytes(), 0, data.length());
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                os.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}
