
public class BlackBoxStarter {
    public static void main(String [] args) throws InterruptedException {
        // args:
        //  0 - token
        //  1 - duration
        
        String token = args[0];
        int duration = Integer.parseInt(args[1]);
        
        long start = System.currentTimeMillis();
        long now = start;
        
        do {
            System.out.println(token + " " + ((now - start) / 1000));
            Thread.sleep(1000);
            now = System.currentTimeMillis();
        } while (now - start < (duration*1000));
    }
}