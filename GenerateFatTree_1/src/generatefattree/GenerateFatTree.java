/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package generatefattree;

import java.util.Scanner;

/**
 *
 * @author Dell
 */
public class GenerateFatTree {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        System.out.println("Input number of Switch \'s gates : \n");
        Scanner scanner = new Scanner(System.in);        
        int k = scanner.nextInt();
        FatTreeGraph fatTree = new FatTreeGraph(k);
    }
    
}
